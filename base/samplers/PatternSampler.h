#ifndef PATTERN_SAMPLER_H
#define PATTERN_SAMPLER_H

#include "../Reader.h"
#include "../Random.h"
#include "../Corrupt.h"

#include <string>

#include "../triple/main.h"
#include "../patterns/main.h"

#include "../corruption/DefaultCorruptionStrategy.h"

#include "LocalSamplingState.h"
#include "TripleBatch.h"
#include "Sampler.h"

using namespace std;

typedef void * (*PTR)(void *);


struct PatternSampler: Sampler {
    Pattern pattern;
    INT nObservedTriplesPerPatternInstance;

    bool bern;
    bool crossSampling;

    PatternSampler(Pattern pattern, INT nObservedTriplesPerPatternInstance, bool bern = false, bool crossSampling = false) {
        this->pattern = pattern;
        this->nObservedTriplesPerPatternInstance = nObservedTriplesPerPatternInstance;
        this->bern = bern;
        this->crossSampling = crossSampling;
    }

    TripleBatch* sample(INT batchSize, INT entityNegativeRate, INT relationNegativeRate, INT headBatchFlag) {
        GlobalSamplingState* globalState = new GlobalSamplingState(
            batchSize, entityNegativeRate, relationNegativeRate, headBatchFlag, pattern, nObservedTriplesPerPatternInstance, 500
        );

        pthread_t* threads = (pthread_t*) malloc(workThreads * sizeof(pthread_t));
        LocalSamplingState* localStates = (LocalSamplingState*) malloc(workThreads * sizeof(LocalSamplingState));

        for (INT thread_index = 0; thread_index < workThreads; thread_index++) {
            localStates[thread_index].id = thread_index;
            localStates[thread_index].globalState = globalState;
            localStates[thread_index].corruptionStrategy = new DefaultCorruptionStrategy(trainList, thread_index, testList, validList);
            pthread_create(&threads[thread_index], NULL, run, (void*)(localStates + thread_index));
        }

        for (INT thread_index = 0; thread_index < workThreads; thread_index++) {
            pthread_join(threads[thread_index], NULL);
        }

        // TODO: free memory

        return new TripleBatch(globalState->triples, globalState->labels, globalState->nTriplesToSample);
    }

    static void* run(void* con) {
        LocalSamplingState* localState = (LocalSamplingState *)(con);
        GlobalSamplingState* state = localState->globalState;

        INT threadIndex = localState->id;
        Triple* triples = state->triples;
        REAL* labels = state->labels;
        INT batchSize = state->batchSize;
        INT entityNegativeRate = state->entityNegativeRate;
        INT relationNegativeRate = state->relationNegativeRate;
        INT headBatchFlag = state->headBatchFlag;
        // std::vector<PatternInstance>** patternInstanceSets = para -> patternInstanceSets;
        vector<PatternInstance>* patternInstances = state->patternInstances;
        INT nObservedTriplesPerPatternInstance = state->nObservedTriplesPerPatternInstance;
        REAL headCorruptionThreshold = state->headCorruptionThreshold;

        CorruptionStrategy* corruptionStrategy = localState->corruptionStrategy;

        bool bern = state->bern;
        bool crossSampling = state->crossSampling;

        INT first_triple_index, last_triple_index;

        if (batchSize % workThreads == 0) {
            first_triple_index = threadIndex * (batchSize / workThreads);
            last_triple_index = (threadIndex + 1) * (batchSize / workThreads);
        } else {
            first_triple_index = threadIndex * (batchSize / workThreads + 1); // Last (incomplete) batch is distributed over all complete batches
            last_triple_index = (threadIndex + 1) * (batchSize / workThreads + 1);
            if (last_triple_index > batchSize) last_triple_index = batchSize;  // The last batch contains fewer elements
        }

        INT patternComponentOffset = batchSize * (1 + entityNegativeRate + relationNegativeRate);

        for (INT batchWiseTripleIndex = first_triple_index; batchWiseTripleIndex < last_triple_index; batchWiseTripleIndex++) {
            // cout << "foo" << (*(patternInstanceSets[nObservedTriplesPerPatternInstance])).size() << endl;
            INT sampled_triple_index = rand_max(threadIndex, (*patternInstances).size());
            // cout << "Sampled triple index = " << sampled_triple_index << endl;
            PatternInstance sampledPatternInstance = (*patternInstances)[sampled_triple_index];
            INT patternComponentIndex = 0;

            for (Triple sampledTriple: sampledPatternInstance.triples) {
                triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex] = sampledTriple;
                labels[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex] = 1;
                // INT last = batchSize;
                // Sample negative triples
                for (INT negativeTripleOffset = 1; negativeTripleOffset <= entityNegativeRate; negativeTripleOffset++) {
                    if (!crossSampling){
                        if (bern) // flag for considering a portion of triples with unique head/tail for those of which there is a given relationship
                            headCorruptionThreshold = 1000 * trainList->relationScore->head[sampledTriple.r] / (
                                trainList->relationScore->tail[sampledTriple.r] + trainList->relationScore->head[sampledTriple.r]
                            );
                        if (randd(threadIndex) % 1000 < headCorruptionThreshold) { // Corrupt TAIL by generating a random number
                            // cout << "corrupting tail" << endl;
                            // cout << "Started corrupting tail" << endl;
                            // cout << trainList->head->left[sampledTriple.h] - 1 << "&&" << trainList->head->right[sampledTriple.h];
                            triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = corruptionStrategy->corruptTail(sampledTriple);
                            // triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = Triple(
                            //     sampledTriple.h, sampledTriple.r, corrupt_head(threadIndex, sampledTriple.h, sampledTriple.r)
                            // );
                            // cout << "Finished corrupting tail" << endl;
                        } else { // Corrupt HEAD
                            // cout << "Started corrupting head" << endl;
                            triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = corruptionStrategy->corruptHead(sampledTriple);
                            // triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = Triple(
                            //     corrupt_tail(threadIndex, sampledTriple.t, sampledTriple.r),  sampledTriple.r, sampledTriple.t
                            // );
                            // cout << "Finished corrupting head" << endl;
                        }
                        labels[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = -1;
                        // last += batchSize; // There will be <batchSize> triples generated by other threads which must be skipped
                    } else  {
                        if(headBatchFlag){ // Corrupt HEAD by using provided flag which specifies which part of triple must be corrupted
                            // cout << "Started corrupting head" << endl;
                            triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = corruptionStrategy->corruptHead(sampledTriple);
                            // triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = Triple(
                            //     corrupt_tail(threadIndex, sampledTriple.t, sampledTriple.r),  sampledTriple.r, sampledTriple.t
                            // );
                            // cout << "Finished corrupting head" << endl;
                        } else { // Corrupt TAIL
                            // cout << "Started corrupting tail" << endl;
                            triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = corruptionStrategy->corruptTail(sampledTriple);
                            // triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = Triple(
                            //     sampledTriple.h, sampledTriple.r, corrupt_head(threadIndex, sampledTriple.h, sampledTriple.r)
                            // );
                            // cout << "Finished corrupting tail" << endl;
                        }
                        labels[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = -1;
                        // last += batchSize;
                    }
                } // end loop for each negative triple with corrupted entity
                // Corrupt relations
                for (INT negativeTripleOffset = 1; negativeTripleOffset <= relationNegativeRate; negativeTripleOffset++) {
                    // cout << "Started corrupting relation" << endl;
                    triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + (entityNegativeRate + negativeTripleOffset) * batchSize] =
                        corruptionStrategy->corruptRelation(sampledTriple);
                    // triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + (entityNegativeRate + negativeTripleOffset) * batchSize] = Triple(
                    //     sampledTriple.h,  corrupt_rel(threadIndex, sampledTriple.h, sampledTriple.t), sampledTriple.t
                    // );
                    // cout << "Finished corrupting relation" << endl;
                    // labels[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + (entityNegativeRate + negativeTripleOffset) * batchSize] = -1;
                    // last += batchSize;
                }
                patternComponentIndex += 1;
            }

            INT observedTripleIndexCounter = 0; // For tracking observed triples in their duplicating locations
            for (INT observedTripleIndex: sampledPatternInstance.observedTripleIndices) {
                if (observedTripleIndexCounter >= nObservedTriplesPerPatternInstance) {
                    break;
                }

                triples[patternComponentOffset * (patternComponentIndex + observedTripleIndexCounter) + batchWiseTripleIndex] =
                    triples[patternComponentOffset * observedTripleIndex + batchWiseTripleIndex];
                labels[patternComponentOffset * (patternComponentOffset + observedTripleIndexCounter) + batchWiseTripleIndex] = 1;
                // labels[patternComponentOffset * observedTripleIndex + batchWiseTripleIndex];

                // INT last = batchSize;

                for (INT negativeTripleOffset = 1; negativeTripleOffset <= entityNegativeRate + relationNegativeRate; negativeTripleOffset++) {
                    triples[patternComponentOffset * (patternComponentIndex + observedTripleIndexCounter) + batchWiseTripleIndex + negativeTripleOffset * batchSize] =
                        triples[patternComponentOffset * observedTripleIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize];
                    labels[patternComponentOffset * (patternComponentIndex + observedTripleIndexCounter) + batchWiseTripleIndex + negativeTripleOffset * batchSize] = -1;
                    // last += batchSize;
                }

                observedTripleIndexCounter++;
            }
        } // end loop for each positive triple in the batch which should be processed by the thread
        pthread_exit(NULL);
    }
};

#endif
