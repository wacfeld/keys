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
    enum State {OPENING, CLOSING, SUSTAIN, IDLE};
    enum State state;

    Blanket(void);
    Blanket(std::string shape);
    ~Blanket(void);

    void keyOn(void);
    void keyOff(void);
    
    int setShape(std::string shape);

    stk::StkFloat lastOut(void) const {return lastFrame_[0];}

    stk::StkFloat tick(void);
    stk::StkFrames &tick(stk::StkFrames &frames, unsigned int channel=0);
protected:
    void sampleRateChanged(stk::StkFloat newRate, stk::StkFloat oldRate);

    std::vector<std::pair<stk::StkFloat, stk::StkFloat>> parsePairs(std::string s);

    // vectors of (time, target) pairs for the when the note is held (opening) and released (closing)
    // behind the scenes the time gets converted to a rate so that prematurely released notes take less time to diminish
    std::vector<std::pair<stk::StkFloat, stk::StkFloat>> opening;
    std::vector<std::pair<stk::StkFloat, stk::StkFloat>> closing;

    // when the state is OPENING or CLOSING, the index tells us what part of the opening/closing vectors to look at
    int index;

    stk::StkFloat sampleRate;
};
#endif
