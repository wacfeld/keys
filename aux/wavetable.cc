// simple envelope + wavetable keyboard setup

#include <Stk.h>
#include <RtAudio.h>

#include "wavetable.h"
#include "matrix.h"

//typedef std::multimap<int, std::unique_ptr<Generator>> wavemap;

void press(wavemap *map, int i) {
    std::unique_ptr<SineWave> si{new SineWave};
    si->setFrequency(freqs[i]*1.3333333);
    map->emplace(i, std::move(si));

    std::unique_ptr<BlitSquare> sq{new BlitSquare};
    sq->setFrequency(freqs[i]);
    map->emplace(i, std::move(sq));
}

void unpress(wavemap *map, int i) {
    map->erase(i);
}

int tick(void *output, void *input, uint nframes, double streamTime, RtAudioStreamStatus status, void *data) {
    Matrix *mat = (Matrix *) data;
    
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

