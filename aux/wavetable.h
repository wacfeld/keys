#ifndef WAVETABLE_H
#define WAVETABLE_H

#include <ADSR.h>
#include <FileLoop.h>
#include <vector>
#include <set>

#include "matrix.h"

class WavData {
public:
    int n;
    Matrix *mat;
    std::set<int> held;

    // ADSR = attack, decay, sustain, release envelope
    std::vector<stk::ADSR> envs;
    std::vector<stk::FileLoop> waves;

    WavData(stk::FileLoop wave, stk::ADSR env, Matrix *mat, double *freqs);
};

// data is a Matrix pointer (see matrix.h)
int wav_tick(void*, void*, unsigned int, double, RtAudioStreamStatus, void* data);

#endif
