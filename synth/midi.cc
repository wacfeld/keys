#include "midi.h"
#include <cassert>

MidiOut::MidiOut() {
    // find the correct port

    out.openPort(0);
}

MidiOut::~MidiOut() {
    delete out;
}

void MidiOut::noteOn(int pitch, int channel) {
    assert channel < 16;
    unsigned char message[3];
    message[0] = 0x90 + channel;
    message[1] = 
}
