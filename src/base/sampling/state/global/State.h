#ifndef SAMPLING_STATE_GLOBAL_STATE_H
#define SAMPLING_STATE_GLOBAL_STATE_H

#include "../../../triple/Triple.h"

namespace meager::main::sampling::state::global {

    struct State {
        INT batchSize;

        INT entityNegativeRate;
        INT relationNegativeRate;

        INT nWorkers;
        bool verbose;

        triple::Triple* triples;
        REAL* labels;

        INT length;

        State(
            INT batchSize, INT entityNegativeRate, INT relationNegativeRate, INT nWorkers = 1, bool verbose = false
        ) {
            this->batchSize = batchSize;
            
            this->entityNegativeRate = entityNegativeRate;
            this->relationNegativeRate = relationNegativeRate;

            this->nWorkers = nWorkers;
            this->verbose = verbose;
        }

        void init() {
            triples = new triple::Triple[length];
            labels = new REAL[length];
        }

    };

}

#endif
