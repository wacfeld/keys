#ifndef WAVETABLE_H
#define WAVETABLE_H

class WavData {
    Envelope *envs;
    FileLoop *waves;
    Matrix *mat;

    WavData(Matrix *);
    ~WavData();
};

// data is a Matrix pointer (see matrix.h)
int tick(void*, void*, unsigned int, double, RtAudioStreamStatus, void* data);

#endif
