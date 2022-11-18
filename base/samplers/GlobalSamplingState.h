#ifndef SAMPLERS_GLOBAL_SAMPLING_STATE_H
#define SAMPLERS_GLOBAL_SAMPLING_STATE_H

#include "../triple/main.h"
#include "../patterns/main.h"

struct GlobalSamplingState {
    Triple* triples;
    REAL* labels;

    vector<PatternInstance>* patternInstances;

    INT batchSize;
    INT entityNegativeRate;
    INT relationNegativeRate;

    INT headBatchFlag;
    INT nObservedTriplesPerPatternInstance;

    INT headCorruptionThreshold;
    INT nTriplesToSample;

    GlobalSamplingState(INT batchSize, INT entityNegativeRate, INT relationNegativeRate, INT headBatchFlag, Pattern pattern, INT nObservedTriplesPerPatternInstance, INT headCorruptionThreshold) {
        PatternDescription patternDescription = patternDescriptions[pattern];

        INT nTriplesPerPatternInstance = patternDescription.nTriplesPerInstance;
        INT nTriplesToSample = (nTriplesPerPatternInstance + nObservedTriplesPerPatternInstance) * batchSize * (1 + entityNegativeRate + relationNegativeRate);

        triples = (Triple*) malloc(nTriplesToSample * sizeof(Triple));
        labels = (REAL*) malloc(nTriplesToSample * sizeof(REAL));

        patternInstances = patternDescription.instanceSets[nObservedTriplesPerPatternInstance];

        this->batchSize = batchSize;
        
        this->entityNegativeRate = entityNegativeRate;
        this->relationNegativeRate = relationNegativeRate;

        this->headBatchFlag = headBatchFlag;
        this->nObservedTriplesPerPatternInstance = nObservedTriplesPerPatternInstance;

        this->nTriplesToSample = nTriplesToSample;

        this->headCorruptionThreshold = headCorruptionThreshold;
    }
};

#endif
