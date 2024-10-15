#ifndef QMIDI_H
#define QMIDI_H
#include "midi.h"

// this class wraps the MidiOut class
// it is specialized for MIDI 24TET output
// it achieves this by using channel 0 for regular white and black keys and pitch bending channel 1 for quarter tones

class QMidiOut {
    // default to grand piano, full volume
    QMidiOut(unsigned inst=0, unsigned vol=127);
    ~QMidiOut();

private:
    MidiOut midi;
};

#endif
