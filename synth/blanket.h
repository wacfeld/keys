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
    
    int setShape(std::string shape);

    enum phase getPhase(void) const;
    stk::StkFloat getLevel(void) const;

    stk::StkFloat tick(void);
    stk::StkFrames &tick(stk::StkFrames &frames, unsigned int channel=0);

protected:
    void sampleRateChanged(stk::StkFloat newRate, stk::StkFloat oldRate);

    stk::StkFloat sampleRate;

    // vectors of (time, target) pairs for the when the note is held (opening) and released (closing)
    std::vector<std::pair<stk::StkFloat, stk::StkFloat>> opening;
    std::vector<std::pair<stk::StkFloat, stk::StkFloat>> closing;

    // state variables

    // if the key is pressed again before the envelope becomes zero, we re-enter the opening phase somewhere in the middle (see getIndex())
    // if the key is released while still in the opening phase, the opening phase will complete before switching immediately to release

    // what phase of the envelope are we in?
    enum phase phase;
    // if the phase is opening or closing, what's our current target? (see the above vectors)
    int index;
    // is the key currently held? (used to determine whether to sustain)
    bool held;
    // what level is the envelope at? (somewhere in [0,1])
    stk::StkFloat level;
};
#endif
