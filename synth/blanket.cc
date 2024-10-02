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
        std::cerr << "Blanket could not find semicolon in shape string \"" << shape << "\"\n";
        return 0;
    }
    auto open = shape.substr(0, i);
    auto close = shape.substr(i);

    opening = parsePairs(open);
    closing = parsePairs(close);

    return 1;
}

std::vector<std::pair<stk::StkFloat, stk::StkFloat>> Blanket::parsePairs(std::string s) {
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

stk::StkFloat Blanket::tick(void) {
    return 0;
}

stk::StkFrames &Blanket::tick(stk::StkFrames &frames, unsigned int channel) {

    return frames;
}
