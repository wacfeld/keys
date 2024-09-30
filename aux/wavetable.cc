// simple envelope + wavetable keyboard setup

#include <Stk.h>
#include <RtAudio.h>

#include "wavetable.h"
#include "utils.h"
#include "matrix.h"

using namespace stk;

WavData::WavData(FileLoop wave, ADSR env, Matrix *mat, double *freqs) {
    // process matrix info
    this->mat = mat;
    this->n = mat->keys;

    // initialize FileLoops and ADSRs
    for(int i = 0; i < this->n; i++) {
        // create a copy of the wave, give it its own frequecny, and reset it for good measure
        this->waves.push_back(wave);
        this->waves[i].setFrequency(freqs[i]);
        //this->waves[i].reset();

        // create corresponding copy of the envelope
        this->envs.push_back(env);
    }
}

int wav_tick(void *output, void *input, uint nframes, double streamTime, RtAudioStreamStatus status, void *data) {
    //ulong a = gettime();
    auto dat = (WavData *) data;

    // update held keys and trigger envelopes accordingly
    for(int i = 0; i < dat->mat->keys; i++) {
        // key was just pressed
        if(dat->mat->buf[i] == 1 && dat->held.count(i) == 0) {
            dat->held.insert(i);
            dat->envs[i].keyOn();
        }
        // key was just released
        else if(dat->mat->buf[i] == 0 && dat->held.count(i) > 0) {
            dat->held.erase(i);
            dat->envs[i].keyOff();
        }
    }

    // compute output
    auto out = (StkFloat *) output;

    StkFrames frames(nframes, 1);

    //compute frames for each note and add to total
    for(int i = 0; i < dat->n; i++) {
        StkFrames wav(nframes, 1);
        StkFrames env(nframes, 1);
        dat->waves[i].tick(wav);
        dat->envs[i].tick(env);
        wav *= env;
        frames += wav;
    }

    // write to output buffer and scale down to be within +/-1
    for(ulong i = 0; i < nframes; i++) {
        out[i] = frames[i] / (dat->n+1);
    }
    //ulong b = gettime();
    //elapsed(a,b);

    return 0;
}

