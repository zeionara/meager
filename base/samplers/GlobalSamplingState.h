#ifndef SAMPLERS_GLOBAL_SAMPLING_STATE_H
#define SAMPLERS_GLOBAL_SAMPLING_STATE_H

#include "../triple/main.h"
#include "../patterns/main.h"
#include "../patterns/PatternDescription.h"


struct GlobalSamplingState {
    Triple* triples;
    REAL* labels;

    vector<PatternInstance>* patternInstances;

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
        PatternDescription patternDescription,
        INT batchSize, INT entityNegativeRate, INT relationNegativeRate, bool headBatchFlag, INT nObservedTriplesPerPatternInstance, INT headCorruptionThreshold,
        bool bern = false, bool crossSampling = false, INT nWorkers = 1
    ) {
        // PatternDescription patternDescription = patternDescriptions[pattern];

        INT nTriplesPerPatternInstance = patternDescription.nTriplesPerInstance;
        INT nTriplesToSample = (nTriplesPerPatternInstance + nObservedTriplesPerPatternInstance) * batchSize * (1 + entityNegativeRate + relationNegativeRate);

        triples = (Triple*) malloc(nTriplesToSample * sizeof(Triple));
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
