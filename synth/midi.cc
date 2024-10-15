#include "midi.h"
#include <cassert>

void MidiOut::listPorts() {
    unsigned int n = out.getPortCount();
    printf("there are %u ports:\n", n);
    for(unsigned int i = 0; i < n; i++) {
        std::string name = out.getPortName(i);
        printf("Port %u: %s\n", i, name.c_str());
    }
}

MidiOut::MidiOut() {
    // find the correct port

    out.openPort(0);
}

void MidiOut::noteOn(int pitch, int channel) {
    assert(channel < 16);
    unsigned char message[3];
    message[0] = 0x90 + channel;
    //message[1] = 
}
