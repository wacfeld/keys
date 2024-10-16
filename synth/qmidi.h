#ifndef QMIDI_H
#define QMIDI_H
#include "midi.h"

// this class wraps the MidiOut class
// it is specialized for MIDI 24TET output
// it achieves this by using channel 0 for regular white and black keys and pitch bending channel 1 for quarter tones

class QMidiOut {
public:
    // default to grand piano, full volume
    QMidiOut(unsigned inst=0, unsigned vol=127);
    ~QMidiOut();

    // 0 -> midi 0
    // 1 -> quarter tone above midi 0
    // 2 -> midi 1
    // 3 -> quarter tone above midi 1
    // etc.
    void noteOn(unsigned note, unsigned vel=127);
    void noteOff(unsigned note, unsigned vel=127);

    void setVol(unsigned vol);
    void setInst(unsigned inst);

private:
    MidiOut midi;
};

#endif
