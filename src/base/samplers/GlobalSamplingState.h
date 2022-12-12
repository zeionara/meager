#ifndef SAMPLERS_GLOBAL_SAMPLING_STATE_H
#define SAMPLERS_GLOBAL_SAMPLING_STATE_H

#include "../triple/main.h"
#include "../triple/pattern/main.h"
#include "../triple/pattern/description/Description.h"


struct GlobalSamplingState {
    triple::Triple* triples;
    REAL* labels;

    vector<triple::pattern::Instance>* patternInstances;

    INT batchSize;
    INT entityNegativeRate;
    INT relationNegativeRate;

    INT nObservedTriplesPerPatternInstance;
    bool headBatchFlag;

    INT headCorruptionThreshold;
    INT nTriplesToSample;

    INT nWorkers;

    bool bern;
    bool crossSampling;

    GlobalSamplingState(
        triple::pattern::description::Description patternDescription,
        INT batchSize, INT entityNegativeRate, INT relationNegativeRate, bool headBatchFlag, INT nObservedTriplesPerPatternInstance, INT headCorruptionThreshold,
        bool bern = false, bool crossSampling = false, INT nWorkers = 1, bool verbose = false
    ) {
        // PatternDescription patternDescription = patternDescriptions[pattern];

        INT nTriplesPerPatternInstance = patternDescription.nTriplesPerInstance;
        INT nTriplesToSample = (nTriplesPerPatternInstance + nObservedTriplesPerPatternInstance) * batchSize * (1 + entityNegativeRate + relationNegativeRate);

        // if (verbose) {
        //     cout << "n triples to sample = " << nTriplesToSample << endl;
        // }

        triples = (triple::Triple*) malloc(nTriplesToSample * sizeof(triple::Triple));
        labels = (REAL*) malloc(nTriplesToSample * sizeof(REAL));

        patternInstances = patternDescription.instanceSets[nObservedTriplesPerPatternInstance];

        this->batchSize = batchSize;
        
        this->entityNegativeRate = entityNegativeRate;
        this->relationNegativeRate = relationNegativeRate;

        this->nObservedTriplesPerPatternInstance = nObservedTriplesPerPatternInstance;
        this->headBatchFlag = headBatchFlag;

        this->nTriplesToSample = nTriplesToSample;

        this->headCorruptionThreshold = headCorruptionThreshold;

        this->bern = bern;
        this->crossSampling = crossSampling;

        this->nWorkers = nWorkers;
    }
};

#endif
