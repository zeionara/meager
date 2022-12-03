#ifndef SAMPLERS_DEFAULT_LOCAL_SAMPLING_STATE_H
#define SAMPLERS_DEFAULT_LOCAL_SAMPLING_STATE_H

// #include "state/RandomizationState.h"
#include "randomizer/Randomizer.h"

#include "GlobalSamplingState.h"
#include "../corruption/DefaultCorruptionStrategy.h"


template <typename T>
struct DefaultLocalSamplingState {
    INT id;
    GlobalSamplingState* globalState;
    Randomizer<INT>* randomizer;

    DefaultCorruptionStrategy<T>* corruptionStrategy;

    DefaultLocalSamplingState(INT id, GlobalSamplingState* globalState, DefaultCorruptionStrategy<T>* corruptionStrategy, Randomizer<INT>* randomizer) {
        this->id = id;
        this->globalState = globalState;
        this->corruptionStrategy = corruptionStrategy;
        this->randomizer = randomizer;
    }
};

#endif
