#ifndef SAMPLING_STATE_LOCAL_STATE_H
#define SAMPLING_STATE_LOCAL_STATE_H

#include "../global/State.h"
#include "../../../corruption/Strategy.h"

namespace meager::main::sampling::state::local {

    template <typename T>
    struct State {
        INT id;
        global::State* globalState;

        corruption::Strategy<T>* corruptionStrategy;

        State(INT id, global::State* globalState, corruption::Strategy<T>* corruptionStrategy) {
            this->id = id;
            this->globalState = globalState;
            this->corruptionStrategy = corruptionStrategy;
        }
    };

}

#endif
