// simple envelope + wavetable keyboard setup

#include <Stk.h>
#include <RtAudio.h>
#include <set>

#include "wavetable.h"
#include "matrix.h"

WavData::WavData(stk::FileLoop wave, stk::ADSR env, Matrix *mat, double *freqs) {
    // process matrix info
    this->mat = mat;
    int n = mat->keys;

    // initialize FileLoops and ADSRs
    for(int i = 0; i < n; i++) {
        // create a copy of the wave, give it its own frequecny, and reset it for good measure
        this->waves.push_back(wave);
        this->waves[i].setFrequency(freqs[i]);
        this->waves[i].reset();

        // create corresponding copy of the envelope
        this->envs.push_back(stk::ADSR());
    }
}

int wav_tick(void *output, void *input, uint nframes, double streamTime, RtAudioStreamStatus status, void *data) {
    dat = (WavData *) data;

    std::set<int> held; // set of currently held note indices
    
    for(int i = 0; i < mat->keys; i++) {
        // key was just pressed
        if(buf[i] == 1 && map.count(i) == 0) {
            press(&map, i);
        }
        // key was just released
        else if(buf[i] == 0 && map.count(i) > 0) {
            unpress(&map, i);
        }
    }

    StkFloat *out = (StkFloat *) output;
    StkFrames frames(nframes, 1);


    //ulong a = gettime();
    for(auto it = map.begin(); it != map.end(); it++) {
        StkFrames voice(nframes, 1);
        it->second->tick(voice);
        frames += voice;
    }
    //ulong b = gettime();
    //elapsed(a,b);
    
    for(ulong i = 0; i < nframes; i++) {
        out[i] = frames[i] / (mat->keys+1);
    }
    return 0;
}

