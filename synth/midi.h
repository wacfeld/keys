#ifndef MIDI_H
#define MIDI_H
#include <RtMidi.h>

// a general class which provides a nice interface to RtMidi functionality

class MidiOut {
public:
    MidiOut(void);

    void message(int n, ...);

    void noteOn(unsigned note, unsigned vel, unsigned channel=0);
    void noteOff(unsigned note, unsigned vel, unsigned channel=0);
    void setInst(unsigned inst, unsigned channel=0);
    void setVol(unsigned vol, unsigned channel=0);
    void listPorts();

private:
    // find the port number of the MIDI port we want to use
    unsigned int getPortNum(void);
    RtMidiOut out;
};

#endif
