#ifndef WAVETABLE_H
#define WAVETABLE_H

#include <ADSR.h>
#include <FileLoop.h>
#include <vector>

#include "matrix.h"

class WavData {
    // ADSR = attack, decay, sustain, release envelope
    Matrix *mat;
    std::vector<stk::ADSR> envs;
    std::vector<stk::FileLoop> waves;

    WavData(stk::FileLoop wave, stk::ADSR env, Matrix *mat, double *freqs);
};

// data is a Matrix pointer (see matrix.h)
int tick(void*, void*, unsigned int, double, RtAudioStreamStatus, void* data);

#endif
