// An generalized ADSR envelope
// Unlimited piecewise components

#ifndef BLANKET_H
#define BLANKET_H

#include <Generator.h>
#include <string>
#include <vector>
#include <iostream>

class Blanket: public stk::Generator {
public:
    enum phase {OPENING, CLOSING, SUSTAIN, IDLE};

    Blanket(void);
    Blanket(std::string shape);
    ~Blanket(void);

    void keyOn(void);
    void keyOff(void);
    enum phase getPhase(void);
    
    int setShape(std::string shape);

    stk::StkFloat lastOut(void) const {return lastFrame_[0];}

    stk::StkFloat tick(void);
    stk::StkFrames &tick(stk::StkFrames &frames, unsigned int channel=0);
protected:
    void sampleRateChanged(stk::StkFloat newRate, stk::StkFloat oldRate);

    stk::StkFloat sampleRate;

    // vectors of (time, target) pairs for the when the note is held (opening) and released (closing)
    std::vector<std::pair<stk::StkFloat, stk::StkFloat>> opening;
    std::vector<std::pair<stk::StkFloat, stk::StkFloat>> closing;

    // state variables

    // if the key is released while still in the opening phase, the opening phase will complete before switching to release
    enum Phase phase;
    bool held;
    int index;
};
#endif
