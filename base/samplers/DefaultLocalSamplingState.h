#ifndef SAMPLERS_DEFAULT_LOCAL_SAMPLING_STATE_H
#define SAMPLERS_DEFAULT_LOCAL_SAMPLING_STATE_H

#include "GlobalSamplingState.h"
#include "../corruption/DefaultCorruptionStrategy.h"


template <typename T>
struct DefaultLocalSamplingState {
    INT id;
    GlobalSamplingState* globalState;

    DefaultCorruptionStrategy<T>* corruptionStrategy;

    DefaultLocalSamplingState(INT id, GlobalSamplingState* globalState, DefaultCorruptionStrategy<T>* corruptionStrategy) {
        this->id = id;
        this->globalState = globalState;
        this->corruptionStrategy = corruptionStrategy;
    }
};

#endif
