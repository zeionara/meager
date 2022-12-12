#ifndef SAMPLING_STATE_LOCAL_RANDOMIZED_H
#define SAMPLING_STATE_LOCAL_RANDOMIZED_H

// #include "state/RandomizationState.h"
// #include "randomizer/Randomizer.h"

// #include "../global/GlobalSamplingState.h"
#include "../../../corruption/Strategy.h"
#include "State.h"

namespace meager::main::sampling::state::local {

    // template <typename T, typename R>
    template <typename R>
    struct Randomized: State<R> {
        Randomizer<R>* randomizer;

        Randomized(INT id, global::State* globalState, corruption::Strategy<R>* corruptionStrategy, Randomizer<R>* randomizer): State<R>(id, globalState, corruptionStrategy) {
            this->randomizer = randomizer;
        }
    };

}

#endif
