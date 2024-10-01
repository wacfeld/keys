// An generalized ADSR envelope
// Unlimited piecewise components

#ifndef BLANKET_H
#define BLANKET_H

#include <Generator.h>
#include <string>

namespace stk {

class Blanket: public Generator {
public:
    Blanket(void);
    Blanket(std::string shape);
    ~Blanket(void);

    void keyOn(void);
    void keyOff(void);
    
    void setShape(std::string shape);

    StkFloat lastOut(void) const {return lastFrame_[0];}

    StkFloat tick(void);
    StkFrames &tick(StkFrames &frames, unsigned int channel=0);
protected:
};

}

#endif
