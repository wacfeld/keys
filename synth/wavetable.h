#ifndef WAVETABLE_H
#define WAVETABLE_H

#include <FileLoop.h>
#include <vector>
#include <set>

#include "matrix.h"
#include "blanket.h"

class WavData {
public:
    int n;
    Matrix *mat;
    std::set<int> held;

    std::vector<Blanket> envs;
    std::vector<stk::FileLoop> waves;

    WavData(stk::FileLoop wave, Blanket env, Matrix *mat, double *freqs);
};

// data is a Matrix pointer (see matrix.h)
int wav_tick(void*, void*, unsigned int, double, RtAudioStreamStatus, void* data);

#endif
