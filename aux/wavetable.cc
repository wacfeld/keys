// simple envelope + wavetable keyboard setup

#include <Stk.h>
#include <RtAudio.h>
#include <Envelope.h>

#include "wavetable.h"
#include "matrix.h"

WavData::WavData(Matrix *mat, double freqs) {
    this->mat = mat;
    int n = mat->keys;
    this->envs = new Envelope[n];
    this->
}

WavData wav_init(Matrix *mat) {
    int n = mat->keys;
    WavData wd;
    wd.envs = new Envelope
}

int wav_tick(void *output, void *input, uint nframes, double streamTime, RtAudioStreamStatus status, void *data) {
    static Envelope 
    Matrix *mat = (Matrix *) data;

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

