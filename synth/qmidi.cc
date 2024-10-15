#include "qmidi.h"

typedef unsigned int uint;

QMidiOut::QMidiOut(uint inst, uint vol) {
    // set instruments
    midi.setInst(inst, 0);
    midi.setInst(inst, 1);

    // set volume
    midi.setVol(vol, 0);
    midi.setVol(vol, 1);

    // pitch bend channel 1 up by a quarter tone permanently
    midi.pitchBend(0x2800, 1);
}

QMidiOut::~QMidiOut() {
    // reset pitch bend
    midi.pitchBend(0x2000, 1);
}
