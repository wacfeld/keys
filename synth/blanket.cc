#include "blanket.h"

typedef std::vector<std::pair<stk::StkFloat, stk::StkFloat>> pairvec;

// default constructor sets the most simple envelope possible
Blanket::Blanket(void) {
    held = false;
    phase = IDLE;
    index = 0;

    // instant attack with a target/sustain of 1
    opening.emplace_back(0, 1);
    // instant release
    closing.emplace_back(0, 0);

    Stk::addSampleRateAlert(this);
}

Blanket::Blanket(std::string shape) {
    held = false;
    phase = IDLE;
    index = 0;

    int success = setShape(shape);
    // if shape string was invalid, default to simple envelope
    if(!success) {
        opening.emplace_back(0, 1);
        closing.emplace_back(0, 0);
    }

    Stk::addSampleRateAlert(this);
}

Blanket::~Blanket(void) {
    Stk::removeSampleRateAlert(this);
}

void Blanket::sampleRateChanged(stk::StkFloat newRate, stk::StkFloat oldRate) {
    if(!ignoreSampleRateChange_) {
        sampleRate = newRate;
    }
}

static bool pairsValid(pairvec pairs) {
    for(auto pair : pairs) {
        if(pair.first < 0) {
            std::cerr << "Warning: time must be non-negative, Blanket instance received " << pair.first << std::endl;
            return false;
        }
        if(pair.second < 0 || pair.second > 1) {
            std::cerr << "Warning: target must be be between 0 and 1, Blanket instance received " << pair.second << std::endl;
            return false;
        }
    }
    return true;
}

static pairvec parsePairs(std::string s) {
    // split by commas
    std::vector<std::string> tokens;
    size_t pos = 0;
    while((pos = s.find(",")) != s.npos) {
        auto token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos+1);
    }

    // split by colons
    pairvec pairs;
    double time, target;
    for(size_t i = 0; i < tokens.size(); i++) {
        sscanf(tokens[i].c_str(), "%lf:%lf", &time, &target);
        pairs.emplace_back(time, target);
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
//
// setShape() returns 1 on success and 0 on failure
int Blanket::setShape(std::string shape) {
    // get strings on either side of the semicolon
    size_t i = shape.find(";");
    if(i == shape.npos) {
        std::cerr << "Blanket: could not find semicolon in shape string \"" << shape << "\"\n";
        return 0;
    }

    auto open = parsePairs(shape.substr(0, i));
    auto close = parsePairs(shape.substr(i));

    if(!pairsValid(open) || !pairsValid(close)) {
        return 0;
    }

    if(open.size() == 0) {
        std::cerr << "Blanket: opening shape cannot be empty\n";
        return 0;
    }

    opening = open;
    closing = close;

    // if closing is empty or ends with nonzero target, infer an instantaneous drop to 0
    if(closing.size() == 0 || closing.back().second != 0) {
        closing.emplace_back(0, 0);
    }

    return 1;
}

// because we allow so much freedom in defining the shape of the envelope, entering it from a non-zero level can be nontrivial.
// let f(x) be the function of the shape of the opening portion of the envelope. let C be the current value of the envelope. keyOn() does the following:
//
// - if the line y=C intersects with f(x), start on the leftmost intersection point
// - if they do not intersect, start at the peak of the graph f(x)
//
// getIndex() implements the above logic
size_t getIndex(stk::StkFloat level, pairvec pairs) {
    if(pairs.size() == 0) {
        std::cerr << "unreachable state in Blanket::getIndex(): pairs is empty\n";
        return 0;
    }

    // extract the targets into a vector for easier logic below
    std::vector<stk::StkFloat> targets;
    // append an implicit zero (getIndex() is only used for opening, not closing)
    targets.push_back(0);
    for(auto pair : pairs) {
        targets.push_back(pair.second);
    }

    // find first intersection point
    for(size_t i = 0; i < targets.size()-1; i++) {
        stk::StkFloat before=targets[i], after=targets[i+1];
        if(before <= level && level < after) {
            return i;
        }
    }

    // if no intersection points, find max (break ties with leftmost)
    // remove implicit zero from before
    targets.erase(targets.begin());
    size_t maxind = 0;
    for(size_t i = 1; i < pairs.size(); i++) {
        if(targets[i] > targets[maxind]) {
            maxind = i;
        }
    }
    
    return maxind;
}

void Blanket::keyOn() {
    // update index
    switch(phase) {
    case OPENING:
    case SUSTAIN: // should not be possible under normal operation but we account for it anyway after printing a warning
        std::cerr << "Warning: Blanket::keyOn() called while in SUSTAIN phase\n";
        // FALLTHROUGH
    case CLOSING:
        index = getIndex(level, opening);
        break;
    case IDLE:
        index = 0;
        break;
    }
    
    // update other state variables (do not touch level)
    held = true;
    phase = OPENING;
}

void Blanket::keyOff() {
    held = false;
    switch(phase) {
    case SUSTAIN: // normal release
        phase = CLOSING;
        index = 0;
        break;
    case CLOSING: // should not be possible
        std::cerr << "Warning: Blanket::keyOff() called while in CLOSING phase\n";
        break;
    case IDLE: // also should not be possible
        std::cerr << "Warning: Blanket::keyOff() called while in IDLE phase\n";
        break;
    case OPENING: // early release, do nothing
        ;
    }
}

stk::StkFloat Blanket::tick(void) {
    return 0;
}

stk::StkFrames &Blanket::tick(stk::StkFrames &frames, unsigned int channel) {

    return frames;
}
