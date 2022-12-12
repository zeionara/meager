#ifndef SAMPLERS_DEFAULT_LOCAL_SAMPLING_STATE_H
#define SAMPLERS_DEFAULT_LOCAL_SAMPLING_STATE_H

// #include "state/RandomizationState.h"
// #include "randomizer/Randomizer.h"

#include "GlobalSamplingState.h"
#include "../corruption/DefaultStrategy.h"

template <typename T, typename R>
struct DefaultLocalSamplingState {
    INT id;
    GlobalSamplingState* globalState;
    Randomizer<R>* randomizer;

    corruption::DefaultStrategy<T, R>* corruptionStrategy;

    DefaultLocalSamplingState(INT id, GlobalSamplingState* globalState, corruption::DefaultStrategy<T, R>* corruptionStrategy, Randomizer<R>* randomizer) {
        this->id = id;
        this->globalState = globalState;
        this->corruptionStrategy = corruptionStrategy;
        this->randomizer = randomizer;
    }
};

#endif
