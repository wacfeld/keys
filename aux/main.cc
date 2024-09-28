#define __OS_LINUX__

#include <SineWave.h>
#include <RtAudio.h>

#include "matrix.h"

#define NKEYS 25

// e to e containing middle c
double freqs[NKEYS] = {
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

using namespace stk;
typedef unsigned int uint;

// make an alias for the callback function prototype used by stk
typedef int (*callback)(void*, void*, uint, double, RtAudioStreamStatus, void*);


int tick(void *output, void *input, uint nframes, double streamTime, RtAudioStreamStatus status, void *data) {
    SineWave *sine = (SineWave *) data;
    StkFloat *out = (StkFloat *) output;
    for(unsigned long int i = 0; i < nframes; i++) {
        *out = sine->tick();
        out++;
    }
    return 0;
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

int main()
{
    //RtAudio out;
    //init(&out, tick, data);

    //if(out.startStream()) {
    //    std::cout << out.getErrorText() << std::endl;
    //    exit(1);
    //}

    //cleanup(&out);

    return 0;
}
