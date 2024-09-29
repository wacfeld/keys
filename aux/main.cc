#define __OS_LINUX__

#include <SineWave.h>
#include <RtAudio.h>
#include <map>
#include <signal.h>
#include <atomic>

#include "matrix.h"
#include "utils.h"

#define N_KEYS 25
#define N_OUT 5
#define N_IN 5

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
    Stk::setSampleRate(44100.0);
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

int tick(void *output, void *input, uint nframes, double streamTime, RtAudioStreamStatus status, void *data) {
    //ulong a = gettime();
    static std::map<int, SineWave> map;
    auto buf = (std::atomic<char> *) data;
    
    for(int i = 0; i < N_KEYS; i++) {
        // key was just pressed
        if(buf[i] == 1 && map.count(i) == 0) {
            map[i] = SineWave();
            map[i].setFrequency(freqs[i]);
        }
        // key was just released
        else if(buf[i] == 0 && map.count(i) > 0) {
            map.erase(i);
        }
    }

    StkFloat *out = (StkFloat *) output;

    for(unsigned long int i = 0; i < nframes; i++) {
        StkFloat val = 0;
        for(auto it = map.begin(); it != map.end(); it++) {
            val += it->second.tick();
        }
        *out = val / (N_KEYS+1);
        out++;
    }
    //ulong b = gettime();
    //elapsed(a,b);
    return 0;
}

int main()
{
    RtAudio out;
    
    std::atomic<char> buf[N_KEYS];
    Matrix mat = {.out=N_OUT, .in=N_IN, .keys=N_KEYS, .buf=buf};

    initMatrix(5000000);
    init(&out, tick, (void*) buf);
    gpioSetSignalFunc(SIGINT, handler);

    if(out.startStream()) {
        std::cout << out.getErrorText() << std::endl;
        exit(1);
    }

    while(!stop) {
        poll(&mat);
    }

    cleanup(&out);
    cleanupMatrix();
    return 0;
}
