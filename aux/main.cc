#define __OS_LINUX__

#include "SineWave.h"
#include "RtAudio.h"
 
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
