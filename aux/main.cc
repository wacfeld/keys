#define __OS_LINUX__

#include "SineWave.h"
#include "RtAudio.h"

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
    329.6276};

using namespace stk;

typedef unsigned int uint;

int tick(void *output, void *input, uint nframes, double streamTime, RtAudioStreamStatus status, void *data) {
    SineWave *sine = (SineWave *) data;
    StkFloat *out = (StkFloat *) output;
    for(unsigned long int i = 0; i < nframes; i++) {
        *out = sine->tick();
        out++;
    }
    return 0;
}

int main()
{
    Stk::setSampleRate(44100.0);

    SineWave sine;
    RtAudio out;
    
    RtAudio::StreamParameters params;
    params.deviceId = out.getDefaultOutputDevice();
    params.nChannels = 1;
    RtAudioFormat format = (sizeof(StkFloat) == 8) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    uint frames = RT_BUFFER_SIZE;
    std::cout << RT_BUFFER_SIZE << std::endl;
    if(out.openStream(&params, NULL, format, (uint) Stk::sampleRate(), &frames, &tick, (void *)&sine)) {
        std::cout << out.getErrorText() << std::endl;
        goto cleanup;
    }

    sine.setFrequency(440.0);
    if(out.startStream()) {
        std::cout << out.getErrorText() << std::endl;
        goto cleanup;
    }

    char keyhit;
    std::cout << "press enter to quit";
    std::cin.get(keyhit);
    out.closeStream();
cleanup:
    return 0;
}
