#include "qmidi.h"
#include <cassert>

typedef unsigned int uint;

QMidiOut::QMidiOut(uint inst, uint vol) {
    // set instrument and volume
    setInst(inst);
    setVol(vol);

    // pitch bend channel 1 up by a quarter tone permanently
    midi.pitchBend(0x2800, 1);
}

QMidiOut::~QMidiOut() {
    // reset pitch bend
    midi.pitchBend(0x2000, 1);
}

void QMidiOut::setVol(uint vol) {
    midi.setVol(vol, 0);
    midi.setVol(vol, 1);
}

void QMidiOut::setInst(uint inst) {
    midi.setInst(inst, 0);
    midi.setInst(inst, 1);
}

void QMidiOut::noteOn(uint note, uint vel) {
    assert(note <= 255);
    midi.noteOn(note/2, vel, note%2);
}

void QMidiOut::noteOff(uint note, uint vel) {
    assert(note <= 255);
    midi.noteOff(note/2, vel, note%2);
}
