#ifndef MIDI_H
#define MIDI_H
#include <RtMidi.h>

class MidiOut {
public:
    MidiOut(void);

    void message(int n, ...);

    void noteOn(unsigned int note, unsigned int channel=0);
    void noteOff(unsigned int note, unsigned int channel=0);
    void setInst(unsigned int inst, unsigned int channel=0);
    void setVol(unsigned int vol, unsigned int channel=0);
    void listPorts();

private:
    // find the port number of the MIDI port we want to use
    unsigned int getPortNum(void);
    RtMidiOut out;
};

#endif
