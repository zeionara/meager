#ifndef SAMPLERS_LOCAL_SAMPLING_STATE_H
#define SAMPLERS_LOCAL_SAMPLING_STATE_H

#include "GlobalSamplingState.h"
#include "../corruption/CorruptionStrategy.h"


struct LocalSamplingState {
    INT id;
    GlobalSamplingState* globalState;

    CorruptionStrategy* corruptionStrategy;

    LocalSamplingState(INT id, GlobalSamplingState* globalState, CorruptionStrategy* corruptionStrategy) {
        this->id = id;
        this->globalState = globalState;
        this->corruptionStrategy = corruptionStrategy;
    }
};

#endif
