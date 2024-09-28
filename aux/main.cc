#define __OS_LINUX__

#include <SineWave.h>
#include <RtAudio.h>
#include <map>
#include <signal.h>
#include <atomic>

#include "matrix.h"

#define N_KEYS 25
#define N_OUT 5
#define N_IN 5

using namespace stk;
typedef unsigned int uint;

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

std::map<int, SineWave> data[2];
std::atomic<int> data_i;

// ctrl-c handler
void handler(int s) {
    stop = 1;
}

void init(RtAudio *out, callback f) {
    Stk::setSampleRate(44100.0);
    RtAudio::StreamParameters params;
    params.deviceId = out->getDefaultOutputDevice();
    params.nChannels = 1;
    RtAudioFormat format = (sizeof(StkFloat) == 8) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    uint frames = RT_BUFFER_SIZE;

    if(out->openStream(&params, NULL, format, (uint) Stk::sampleRate(), &frames, f, NULL)) {
        std::cout << out->getErrorText() << std::endl;
        exit(1);
    }
}

void cleanup(RtAudio *out) {
    if(out->isStreamOpen()) {
        out->closeStream();
    }
}

int tick(void *output, void *input, uint nframes, double streamTime, RtAudioStreamStatus status, void *temp) {
    std::map<int, SineWave> *dat = &data[data_i];
    StkFloat *out = (StkFloat *) output;

    for(unsigned long int i = 0; i < nframes; i++) {
        StkFloat val = 0;
        for(auto it = dat->begin(); it != dat->end(); it++) {
            val += it->second.tick();
        }
        *out = val / (N_KEYS+1);
        out++;
    }
    return 0;
}

// poll the matrix and update the data accordingly
void update(Matrix *mat) {
    // make the unused map the current one and copy everything from the other map
    int cur = 1-data_i;
    int old = data_i;
    data[cur] = data[old];

    // check the status of every key against the hash map
    int changed = 0;
    poll(mat);
    //printmat(mat);
    for(int i = 0; i < mat->keys; i++) {
        // key was just pressed, add a sine to the map
        if(mat->buf[i] == 1 && data[cur].count(i) == 0) {
            changed = 1;
            data[cur][i] = SineWave();
            data[cur][i].setFrequency(freqs[i]);
        }

        // key was just released, remove it from the map
        else if(mat->buf[i] == 0 && data[cur].count(i) > 0) {
            changed = 1;
            data[cur].erase(i);
        }
    }

    if(changed) {
        data_i = cur;
    }
}

int main()
{
    data_i = 0;
    RtAudio out;
    
    char buf[N_KEYS];
    Matrix mat = {.out=N_OUT, .in=N_IN, .keys=N_KEYS, .buf=buf};

    initMatrix();
    init(&out, tick);
    gpioSetSignalFunc(SIGINT, handler);

    if(out.startStream()) {
        std::cout << out.getErrorText() << std::endl;
        exit(1);
    }

    while(!stop) {
        update(&mat);
    }

    cleanup(&out);
    cleanupMatrix();
    return 0;
}
