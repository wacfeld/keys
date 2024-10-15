#ifndef MIDI_H
#define MIDI_H
#include <RtMidi.h>

class MidiOut {
public:
    MidiOut(void);
    ~MidiOut(void);

    void noteOn();
    void noteOff();

private:
    RtMidiOut *out;
};

#endif
