#ifndef SAMPLING_STATE_GLOBAL_PATTERN_H
#define SAMPLING_STATE_GLOBAL_PATTERN_H

// #include "../../../triple/main.h"
// #include "../../../triple/pattern/main.h"
#include "../../../triple/pattern/description/Description.h"

#include "State.h"


namespace meager::main::sampling::state::global {

    struct Pattern: State {
        vector<triple::pattern::Instance>* patternInstances;

        INT nObservedTriplesPerPatternInstance;

        INT headCorruptionThreshold;

        bool headBatchFlag;
        bool bern;
        bool crossSampling;

        Pattern(
            triple::pattern::description::Description patternDescription,
            INT batchSize, INT entityNegativeRate, INT relationNegativeRate, bool headBatchFlag, INT nObservedTriplesPerPatternInstance, INT headCorruptionThreshold,
            bool bern = false, bool crossSampling = false, INT nWorkers = 1, bool verbose = false
        ): State(batchSize, entityNegativeRate, relationNegativeRate, nWorkers, verbose) {
            length = (patternDescription.nTriplesPerInstance + nObservedTriplesPerPatternInstance) * batchSize * (1 + entityNegativeRate + relationNegativeRate);

            init();

            patternInstances = patternDescription.instanceSets[nObservedTriplesPerPatternInstance];

            this->nObservedTriplesPerPatternInstance = nObservedTriplesPerPatternInstance;

            this->headCorruptionThreshold = headCorruptionThreshold;

            this->headBatchFlag = headBatchFlag;
            this->bern = bern;
            this->crossSampling = crossSampling;
        }
    };

}

#endif
