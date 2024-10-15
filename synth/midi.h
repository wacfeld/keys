#ifndef MIDI_H
#define MIDI_H
#include <RtMidi.h>

class MidiOut {
public:
    MidiOut(void);
    ~MidiOut(void);

    void noteOn(int note, int channel=0);
    void noteOff();
    void listPorts();

private:
    RtMidiOut out;
};

#endif
