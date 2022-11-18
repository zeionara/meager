#ifndef SAMPLER_H
#define SAMPLER_H

#include <string>

#include "../triple/main.h"
#include "../patterns/main.h"

#include "LocalSamplingState.h"

using namespace std;

struct TripleBatch {
    Triple* triples;
    REAL* labels;

    INT length;

    INT* head;
    INT* relation;
    INT* tail;

    TripleBatch(Triple* triples, REAL* labels, INT length) {
        head = (INT*) malloc(length * sizeof(INT));
        relation = (INT*) malloc(length * sizeof(INT));
        tail = (INT*) malloc(length * sizeof(INT));

        for (INT i = 0; i < length; i++) {
            Triple triple = triples[i];

            head[i] = triple.h;
            relation[i] = triple.r;
            tail[i] = triple.t;
        }

        this->triples = triples;
        this->labels = labels;

        this->length = length;
    }
};

struct Sampler {
    Pattern pattern;
    INT nObservedTriplesPerPatternInstance;

    Sampler(Pattern pattern, INT nObservedTriplesPerPatternInstance) {
        this->pattern = pattern;
        this->nObservedTriplesPerPatternInstance = nObservedTriplesPerPatternInstance;
    }

    TripleBatch* sample(INT batchSize, INT entityNegativeRate, INT relationNegativeRate, INT headBatchFlag) {
        GlobalSamplingState* globalState = new GlobalSamplingState(batchSize, entityNegativeRate, relationNegativeRate, headBatchFlag, pattern, nObservedTriplesPerPatternInstance, 500);

        pthread_t* threads = (pthread_t*) malloc(workThreads * sizeof(pthread_t));
        LocalSamplingState* localStates = (LocalSamplingState*) malloc(workThreads * sizeof(LocalSamplingState));

        for (INT thread_index = 0; thread_index < workThreads; thread_index++) {
            localStates[thread_index].id = thread_index;
            localStates[thread_index].globalState = globalState;
            pthread_create(&threads[thread_index], NULL, getPatternBatch, (void*)(localStates + thread_index));
        }

        for (INT thread_index = 0; thread_index < workThreads; thread_index++) {
            pthread_join(threads[thread_index], NULL);
        }

        return new TripleBatch(globalState->triples, globalState->labels, globalState->nTriplesToSample);

        // free(pt);
        // free(para);
    }
};

#endif
