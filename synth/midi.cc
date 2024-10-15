#include "midi.h"

MidiOut::MidiOut() {
    try {
        out = new RtMidiOut();
    } catch(RtMidiError &err) {
        puts("MidiOut encountered the following error when trying to initialize:");
        err.printMessage();
        exit(1);
    }
}
