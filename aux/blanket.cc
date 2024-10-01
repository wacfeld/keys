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

    setShape(shape);

    Stk::addSampleRateAlert(this);
}

Blanket::~Blanket(void) {
    Stk::removeSampleRateAlert(this);
}

void Blanket::sampleRateChanged(stk::StkFloat newRate, stk::StkFloat oldRate) {
    if(!ignoreSampleRateChange_) {

    }
}

void Blanket::setShape(std::string shape) {

}

stk::StkFloat Blanket::tick(void) {
    return 0;
}

stk::StkFrames &Blanket::tick(stk::StkFrames &frames, unsigned int channel) {

    return frames;
}
