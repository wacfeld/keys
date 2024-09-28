#define __OS_LINUX__

#include <SineWave.h>
#include <RtAudio.h>
#include <map>

#include "matrix.h"

#define N_KEYS 25
#define N_OUT 5
#define N_IN 5

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

// make an alias for the callback function prototype used by stk (tick() is an example)
typedef int (*callback)(void*, void*, uint, double, RtAudioStreamStatus, void*);

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
    SineWave *sine = (SineWave *) data;
    StkFloat *out = (StkFloat *) output;
    for(unsigned long int i = 0; i < nframes; i++) {
        *out = sine->tick();
        out++;
    }
    return 0;
}

// poll the matrix and update the data accordingly
void update(std::map<int, SineWave> &&data, Matrix *mat) {

    // check the status of every key against the hash map
    poll(mat);
    for(int i = 0; i < mat->keys; i++) {
        // key was just pressed
        if(mat->buf[i] == 1 && data.count(i) == 0) {
            data[i] = SineWave();
            data[i].setFrequency(freqs[i]);
        }

        // key was just released
        else if(mat->buf[i] == 0 && data.count(i) > 0) {
            data.erase(i);
        }
    }
}

int main()
{
    RtAudio out;

    std::map<int, SineWave> data;
    
    char buf[N_KEYS];
    Matrix *mat = {.out=N_OUT, .in=N_IN, .keys=N_KEYS, .buf=buf};

    initMatrix();
    init(&out, tick, (void*) &data);

    //if(out.startStream()) {
    //    std::cout << out.getErrorText() << std::endl;
    //    exit(1);
    //}

    cleanup(&out);

    cleanupMatrix();

    return 0;
}
