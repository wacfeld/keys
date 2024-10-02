#include "blanket.h"

// default constructor sets the most simple envelope possible
Blanket::Blanket(void) {
    state = IDLE;
    index = 0;

    // instant attack with a target/sustain of 1
    opening.emplace_back(0, 1);
    // instant release
    closing.emplace_back(0, 0);

    Stk::addSampleRateAlert(this);
}

Blanket::Blanket(std::string shape) {
    state = IDLE;
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

static bool isValid(std::vector<std::pair<stk::StkFloat, stk::StkFloat>> pairs) {
    for(auto pair : pairs) {
        if(pair.first < 0) {
            std::cerr << "Blanket: time must be non-negative, received " << pair.first << std::endl;
            return false;
        }
        if(pair.second < 0 || pair.second > 1) {
            std::cerr << "Blanket: target must be be between 0 and 1, received " << pair.second << std::endl;
            return false;
        }
    }
    return true;
}

static std::vector<std::pair<stk::StkFloat, stk::StkFloat>> parsePairs(std::string s) {
    // split by commas
    std::vector<std::string> tokens;
    size_t pos = 0;
    while((pos = s.find(",")) != s.npos) {
        auto token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos+1);
    }

    // split by colons
    std::vector<std::pair<stk::StkFloat, stk::StkFloat>> pairs;
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

    if(!isValid(open) || !isValid(close)) {
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

// because we allow so much freedom in defining the shape of the envelope, keyOn() and keyOff() are nontrivial
//
// let f(x) be the function of the shape of the opening portion of the envelope. let C be the current value of the envelope. keyOn() does the following:
//
// - if the line y=C intersects with f(x), start on the leftmost intersection point
// - if they do not intersect, start at the peak of the graph f(x)
//
// the logic is identical for keyOff().
void Blanket::keyOn() {
    // change the state
    state = OPENING;

    // figure out our index
}

stk::StkFloat Blanket::tick(void) {
    return 0;
}

stk::StkFrames &Blanket::tick(stk::StkFrames &frames, unsigned int channel) {

    return frames;
}
