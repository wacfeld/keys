#ifndef MIDI_H
#define MIDI_H
#include <RtMidi.h>

class MidiOut {
public:
    MidiOut(void);

    void message(int n, ...);

    void noteOn(int note, int channel=0);
    void noteOff(int note, int channel=0);
    void setInst(int inst, int channel=0);
    void setVol(int vol, int channel=0);
    void listPorts();

private:
    // find the port number of the MIDI port we want to use
    unsigned int getPortNum(void);
    RtMidiOut out;
};

#endif
