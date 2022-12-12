#ifndef SAMPLERS_DEFAULT_LOCAL_SAMPLING_STATE_H
#define SAMPLERS_DEFAULT_LOCAL_SAMPLING_STATE_H

// #include "state/RandomizationState.h"
// #include "randomizer/Randomizer.h"

#include "GlobalSamplingState.h"
#include "../corruption/DefaultStrategy.h"

template <typename T>
struct DefaultLocalSamplingState {
    INT id;
    GlobalSamplingState* globalState;
    Randomizer<INT>* randomizer;

    corruption::DefaultStrategy<T>* corruptionStrategy;

    DefaultLocalSamplingState(INT id, GlobalSamplingState* globalState, corruption::DefaultStrategy<T>* corruptionStrategy, Randomizer<INT>* randomizer) {
        this->id = id;
        this->globalState = globalState;
        this->corruptionStrategy = corruptionStrategy;
        this->randomizer = randomizer;
    }
};

#endif
