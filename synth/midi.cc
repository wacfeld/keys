#include "midi.h"
#include <cassert>
#include <cstdarg>
#include <array>

typedef unsigned int uint;

MidiOut::MidiOut() {
    // connect to port
    unsigned int n = getPortNum();
    out.openPort(n);
}

MidiOut::~MidiOut() {
    out.closePort();
}

void MidiOut::message(int n, ...) {
    va_list ap;
    va_start(ap, n);

    std::vector<unsigned char> bytes;
    for(int i = 0; i < n; i++) {
        bytes.push_back(va_arg(ap, unsigned int));
    }

    out.sendMessage(&bytes);

    va_end(ap);
}

void MidiOut::listPorts() {
    unsigned int n = out.getPortCount();
    printf("there are %u ports:\n", n);
    for(unsigned int i = 0; i < n; i++) {
        std::string name = out.getPortName(i);
        printf("Port %u: %s\n", i, name.c_str());
    }
}

unsigned int MidiOut::getPortNum() {
    unsigned int n = out.getPortCount();
    for(unsigned int i = 0; i < n; i++) {
        std::string name = out.getPortName(i);

        // check if port name starts with "f_midi"
        if(name.rfind("f_midi", 0) == 0) {
            return i;
        }
    }

    listPorts();
    std::cerr << "MidiOut::getPortNum(): Could not find port name starting with \"f_midi\", returning port number 0\n";
    return 0;
}

void MidiOut::setInst(uint inst, uint channel) {
    assert(channel < 16);
    assert(inst <= 127);
    message(2, 0xC0 + channel, inst);
}

void MidiOut::setVol(uint vol, uint channel) {
    assert(vol <= 127);
    assert(channel < 16);
    message(3, 0xB0 + channel, 7, vol);
}

void MidiOut::pitchBend(uint val, uint channel) {
    assert(channel < 16);
    assert(val < 0x4000);

    // least and most significant 7 bits
    uint L = val & 0x7F;
    uint M = val >> 7;

    message(3, 0xE0 + channel, L, M);
}

void MidiOut::noteOn(uint pitch, uint vel, uint channel) {
    assert(channel < 16);
    assert(pitch <= 127);
    assert(vel <= 127);
    message(3, 0x90 + channel, pitch, vel);
}

void MidiOut::noteOff(uint pitch, uint vel, uint channel) {
    assert(channel < 16);
    assert(pitch <= 127);
    assert(vel <= 127);
    message(3, 0x80 + channel, pitch, vel);
}
