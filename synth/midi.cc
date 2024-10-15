#include "midi.h"
#include <cassert>
#include <cstdarg>
#include <array>

void MidiOut::message(int n, ...) {
    va_list ap;
    va_start(ap, n);

    std::vector<unsigned char> bytes;
    for(int i = 0; i < n; i++) {
        bytes.push_back(va_arg(ap, int));
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

void MidiOut::setInst(int inst, int channel) {
    //unsigned char 
}

MidiOut::MidiOut() {
    // connect to port
    unsigned int n = getPortNum();
    out.openPort(n);


}

void MidiOut::noteOn(int pitch, int channel) {
    assert(channel < 16);
    unsigned char message[3];
    message[0] = 0x90 + channel;
    //message[1] = 
}
