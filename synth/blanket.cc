#include "blanket.h"

typedef std::vector<std::pair<long, stk::StkFloat>> pairvec;

// default constructor sets the most simple envelope possible
Blanket::Blanket(stk::StkFloat sampleRate) {
    // initialize variables
    phase = IDLE;
    time = 0;
    held = false;
    level = 0;
    this->sampleRate = sampleRate;

    // set instant attack and release as default
    setShape("0:1;0:0");
}

enum Blanket::phase Blanket::getPhase() const {
    return phase;
}

stk::StkFloat Blanket::getLevel() const {
    return level;
}

// input is seconds, output is samples
long Blanket::calcSamples(stk::StkFloat s) {
    // minimum allowed delay is 0, so for example calcDelay(0) == 1
    long d = s * sampleRate;
    return (d == 0) ? 1 : d;
}

// custom exception for when parsing fails input validation
class BlanketParseException: public std::exception {
public:
    char *what() {
        return (char *)"Blanket parsing error";
    }
};

pairvec Blanket::parsePairs(std::string s) {
    // split by commas
    std::vector<std::string> tokens;
    size_t pos = 0;
    while((pos = s.find(",")) != s.npos) {
        auto token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos+1);
    }
    tokens.push_back(s);

    // keep track of the time in samples
    pairvec pairs;
    long samp = 0;

    // split by colons
    for(size_t i = 0; i < tokens.size(); i++) {
        stk::StkFloat time, target;
        sscanf(tokens[i].c_str(), "%lf:%lf", &time, &target);

        // do input validation
        if(time < 0) {
            std::cerr << "Warning: time must be non-negative, Blanket received " << time << std::endl;
            throw BlanketParseException();
        }
        if(target < 0 || target > 1) {
            std::cerr << "Warning: target must be between 0 and 1, Blanket received " << target << std::endl;
            throw BlanketParseException();
        }

        samp += calcSamples(time);
        pairs.emplace_back(samp, target);
    }

    return pairs;
}


// `shape` has form "opening;closing",
// where opening and closing are comma separated lists of pairs,
// where a pair has form "time:target",
// where time and target are floats.
//
// notes:
// - all times must be non-negative (measured in seconds) and all targets must be in [0,1]
// - a time of zero means an instantaneous change to the target
// - opening must have at least one pair
// - the final target in opening is the sustain value
// - if closing has no pairs, or has a nonzero final target, then it will automatically drop to zero after exhausting all pairs
// - the semicolon must still be present if closing is empty
// - spaces are optional
//
// for example, "0.1:1, 0.2:0.5, 5:0.1; 1:0.05, 10:0" means:
// - 0.1s to get to 1
// - 0.2s to get to 0.5
// - 5s to get to 0.1
// - sustain at 0.1
// - 1s to get to 0.05
// - 10s to get to 0
//
// "0:1;" means:
// - instantaneously go to 1
// - sustain at 1
// - instantaneously go to 0
void Blanket::setShape(std::string shape) {
    // get strings on either side of the semicolon
    size_t i = shape.find(";");
    if(i == shape.npos) {
        std::cerr << "Blanket::setShape(): could not find semicolon in shape string \"" << shape << "\"\n";
        return;
    }

    pairvec open;
    pairvec close;
    try {
        open = parsePairs(shape.substr(0, i));
        close = parsePairs(shape.substr(i+1));
    } catch (BlanketParseException &e) {
        std::cerr << "Blanket::setShape() failed\n";
        return;
    }

    if(open.empty()) {
        std::cerr << "Blanket::setShape(): opening shape cannot be empty\n";
        return;
    }

    // set start of opening to level 0
    open.emplace(open.begin(), 0, 0);
    // set start of closing to sustain level
    close.emplace(close.begin(), 0, open.back().second);

    // make sure closing ends at level 0
    if(close.empty() || close.back().second != 0) {
        close.emplace_back(close.back().first+1, 0);
    }

    // assign respective fields
    opening = open;
    closing = close;
}

// when repressing or releasing early, we may have to enter the opening/closing phase from a nonzero time
// this function calculates that time
//
// let f(x) be the function of the shape of the opening portion of the envelope. let C be the current value of the envelope. we do the following:
//
// - if the line y=C intersects with f(x), return the time at the leftmost intersection point
// - if they do not intersect, start at the left peak of the graph f(x)
//
// this second rule is kind of ugly but it should not show up often under normal usage (envelopes that mimic real intruments)
long getTime(stk::StkFloat level, pairvec *pairs) {
    if(pairs->size() < 2) {
        std::cerr << "unreachable state in Blanket::getIndex(): pairs has size " << pairs->size() << ", size of at least 2 required\n";
        return 0;
    }

    // find first intersection point
    for(size_t i = 0; i < pairs->size()-1; i++) {
        // unpack values
        long x1 = (*pairs)[i].first, x2 = (*pairs)[i+1].first;
        stk::StkFloat y1=(*pairs)[i].second, y2=(*pairs)[i+1].second;

        // check if level is in [before, after) or (after, before]
        if((y1 <= level && level <= y2) || (y2 <= level && level <= y1)) {
            // interpolate
            long lvl;
            if(y1 == y2) { // avoid divide by zero case when levels are equal
                lvl = x1;
            } else {
                lvl = x1 + (x2-x1) * (level-y1) / (y2-y1);
            }

            // make sure no strange floating point errors arise
            if(lvl < x1) return x1;
            else if(lvl > x2) return x2;
            else return lvl;
        }
    }

    // find leftmost peak
    size_t maxind = 0;
    for(size_t i = 1; i < pairs->size(); i++) {
        if((*pairs)[i] > (*pairs)[maxind]) {
            maxind = i;
        }
    }
    
    return (*pairs)[maxind].first;
}

void Blanket::keyOn() {
    // warn on double press
    if(held) {
        std::cerr << "Warning: Blanket::keyOn() called twice in a row. Ignoring second call\n";
        return;
    }
    held = true;

    // update index
    switch(phase) {
    // early repress
    case OPENING:
    case SUSTAIN: // should not be possible
    case CLOSING:
        time = getTime(level, &opening);
        break;
    // regular repress
    case IDLE:
        time = 0;
        break;
    }

    phase = OPENING;
}

void Blanket::keyOff() {
    // warn on double release
    if(!held) {
        std::cerr << "Warning: Blanket::keyOff() called twice in a row. Ignoring second call\n";
        return;
    }
    held = false;

    switch(phase) {
    case SUSTAIN: // regular release
        phase = CLOSING;
        time = 0;
        break;
    case CLOSING: // should not be possible
    case IDLE: // also should not be possible
    case OPENING: // early release, do nothing
        break;
    }
}

pairvec *Blanket::curPairs() {
    if(phase == OPENING) {
        return &opening;
    } else if(phase == CLOSING) {
        return &closing;
    } else {
        std::cerr << "Blanket::curPairs() called outside of opening or closing\n";
        return nullptr;
    }
}

stk::StkFloat Blanket::getSustainLevel() const {
    return opening.back().second;
}

// we completed one phase, now increment the index or switch to the next phase
void Blanket::nextPhase() {
    if(phase == OPENING) {
        if(held) {
            phase = SUSTAIN;
            time = 0;
            level = getSustainLevel();
        } else {
            phase = CLOSING;
            time = 0;
            level = closing[0].second;
        }
    }

    else if(phase == CLOSING) {
        phase = IDLE;
        time = 0;
        level = 0;
    }

    else {
        std::cerr << "Warning: Blanket::nextPhase() called outside of opening or closing\n";
    }
}

// returns level for current time in current phase
// returns -1 if time has exceeded phase length
stk::StkFloat time2level(pairvec *pairs, long time) {
    if(time > (*pairs).back().first) {
        return -1;
    }

    for(size_t i = 0; i < pairs->size()-1; i++) {
        long x1 = (*pairs)[i].first, x2 = (*pairs)[i+1].first;
        if(x1 <= time && time <= x2) {
            stk::StkFloat y1=(*pairs)[i].second, y2=(*pairs)[i+1].second;
            return y1 + (y2-y1) * (time - x1) / (x2 - x1);
        }
    }

    std::cerr << "Warning: Unreachable state reached in time2level()\n";
    return -1;
}

inline stk::StkFloat Blanket::tick(void) {
    
    // if IDLE or SUSTAIN, return appropriate value
    if(phase == IDLE) {
        level = 0;
        return level;
    } else if(phase == SUSTAIN) {
        level = getSustainLevel();
        return level;
    }

    // otherwise increment time and calculate new level
    pairvec *pairs = curPairs();
    time++;
    stk::StkFloat lvl = time2level(pairs, time);

    if(lvl == -1) {
        nextPhase();
        return level;
    } else {
        level = lvl;
        return level;
    }
}

stk::StkFrames &Blanket::tick(stk::StkFrames &frames, unsigned int channel) {
    stk::StkFloat *samples = &frames[channel];
    unsigned int hop = frames.channels();
    for(unsigned int i = 0; i < frames.frames(); i++) {
        *samples = Blanket::tick();
        samples += hop;
    }
    return frames;
}

void Blanket::printInfo() {
    std::cout << "Sample rate " << sampleRate << std::endl;

    puts("Opening:");
    for(size_t i = 0; i < opening.size(); i++) {
        printf("%ld, %f\n", opening[i].first, opening[i].second);
    }

    puts("Closing:");
    for(size_t i = 0; i < closing.size(); i++) {
        printf("%ld, %f\n", closing[i].first, closing[i].second);
    }
}
