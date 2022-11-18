#ifndef SAMPLERS_LOCAL_SAMPLING_STATE_H
#define SAMPLERS_LOCAL_SAMPLING_STATE_H

#include "GlobalSamplingState.h"

struct LocalSamplingState {
    INT id;
    GlobalSamplingState* globalState;

    LocalSamplingState(INT id, GlobalSamplingState* globalState) {
        this->id = id;
        this->globalState = globalState;
    }
};

#endif
