#define __OS_LINUX__

#include <signal.h>
#include <atomic>
#include <unistd.h>
#include <set>

#include <RtAudio.h>
#include <SineWave.h>

#include "matrix.h"
#include "utils.h"
#include "wavetable.h"
#include "blanket.h"
#include "qmidi.h"

#define N_KEYS 25
#define N_OUT 5
#define N_IN 5

// midi 52 is E (164.8 Hz), multiply that by 2
#define MIDIOFFSET 104

//#define SAMPLERATE 48000
#define SAMPLERATE 44100

using namespace stk;
typedef unsigned int uint;
typedef unsigned long ulong;

// make an alias for the callback function prototype used by stk (tick() is an example)
typedef int (*callback)(void*, void*, uint, double, RtAudioStreamStatus, void*);

// e to e containing middle c
double freqs[N_KEYS] = {
    164.8138, 169.6432,
    174.6141, 179.7307,
    184.9972, 190.4180,
    195.9977, 201.7409,
    207.6523, 213.7370,
    220.0000, 226.4465,
    233.0819, 239.9117,
    246.9417, 254.1776,
    261.6256, 269.2918,
    277.1826, 285.3047,
    293.6648, 302.2698,
    311.1270, 320.2437,
    329.6276
};

static volatile int stop = 0;

// ctrl-c handler
void handler(int s) {
    stop = 1;
}

void init(RtAudio *out, callback f, void *data) {
    Stk::setSampleRate(SAMPLERATE);
    RtAudio::StreamParameters params;
    params.deviceId = out->getDefaultOutputDevice();
    params.nChannels = 1;
    RtAudioFormat format = (sizeof(StkFloat) == 8) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    uint frames = RT_BUFFER_SIZE;

    if(out->openStream(&params, NULL, format, (uint) Stk::sampleRate(), &frames, f, data)) {
        std::cout << out->getErrorText() << std::endl;
        exit(1);
    }
}

void cleanup(RtAudio *out) {
    if(out->isStreamOpen()) {
        out->closeStream();
    }
}

void printframes(StkFrames frames) {
    for(ulong i = 0; i < frames.size(); i++) {
        printf("%f ", frames[i]);
    }
    putchar('\n');
}

void runKeyboard(int argc, char **argv) {
    std::string fname = "waves/sine.raw";
    std::string shape = "0.01:1;0.01:0";
    static char usage[] = "-f: raw file name\n-e: envelope shape\n";

    int opt;
    while((opt = getopt(argc, argv, "e:f:")) != -1) {
        switch(opt) {
        case 'f': fname = optarg; break;
        case 'e': shape = optarg; break;
        default: printf("%s", usage); exit(1);
        }
    }
    
    // initialize elements of WavData (matrix, envelope, waveform)
    std::atomic<char> buf[N_KEYS];
    Matrix mat = {.out=N_OUT, .in=N_IN, .keys=N_KEYS, .buf=buf};
    initMatrix(5000000);
    gpioSetSignalFunc(SIGINT, handler); // override pigpio's signal handler

    FileLoop wave(fname, true);
    Stk::setSampleRate(SAMPLERATE);
    Blanket env(SAMPLERATE);
    env.setShape(shape);
    
    // create WavData
    WavData data(wave, env, &mat, freqs);

    // init() with tick function and data
    RtAudio out;
    init(&out, wav_tick, (void*) &data);

    if(out.startStream()) {
        std::cout << out.getErrorText() << std::endl;
        exit(1);
    }

    QMidiOut qmidi(4);
    std::set<int> held;

    while(!stop) {
        poll(&mat);

        // update midi
        for(int i = 0; i < mat.keys; i++) {
            if(mat.buf[i] == 1 && held.count(i) == 0) {
                held.insert(i);
                qmidi.noteOn(MIDIOFFSET + i);
            }
            else if(mat.buf[i] == 0 && held.count(i) > 0) {
                held.erase(i);
                qmidi.noteOff(MIDIOFFSET + i);
            }
        }
    }

    cleanup(&out);
    cleanupMatrix();
}

void printtick(Blanket *b, int n) {
    for(int i = 0; i < n; i++) {
        auto f = b->tick();
        printf("%f\n", f);
    }
}

void blanketTest() {
    Blanket b(SAMPLERATE);
    b.setShape("0.01:1;0.01:0");
    b.printInfo();
    printtick(&b, 5);
    b.keyOn();
    printtick(&b, 442);
    b.keyOff();
    printtick(&b, 442);
}

int main(int argc, char *argv[])
{
    //MidiOut midi;
    //midi.setInst(4);
    //midi.setVol(100);
    //midi.pitchBend(0x2000);
    //midi.noteOn(64, 100);
    //usleep(500000);
    //midi.noteOff(64, 100);
    //midi.pitchBend(0x2800);
    //midi.noteOn(64, 100);
    //usleep(500000);
    //midi.noteOff(64, 100);
    //
    //QMidiOut qmidi(4);
    //for(int i = 128; i <= 128+24; i++) {
    //    qmidi.noteOn(i, 100);
    //    usleep(500000);
    //    qmidi.noteOff(i, 100);
    //}

    //blanketTest();

    runKeyboard(argc, argv);
    return 0;
}
