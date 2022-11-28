#ifndef PATTERN_SAMPLER_H
#define PATTERN_SAMPLER_H

// #include "../Reader.h"
#include "../Random.h"
// #include "../Corrupt.h"

#include <string>

#include "../triple/main.h"
#include "../patterns/main.h"

#include "../corruption/DefaultCorruptionStrategy.h"

#include "DefaultLocalSamplingState.h"
#include "TripleBatch.h"
#include "Sampler.h"

#include "../global.h"

using namespace std;

typedef void * (*PTR)(void *);


template <typename T>
struct PatternSampler: Sampler<LocalTsvCorpus<long>> {
    Pattern pattern;
    INT nObservedTriplesPerPatternInstance;
    INT nWorkers;

    bool bern;
    bool crossSampling;

    PatternSampler(Pattern pattern, INT nObservedTriplesPerPatternInstance, bool bern = false, bool crossSampling = false, INT nWorkers = 1) {
        this->pattern = pattern;
        this->nObservedTriplesPerPatternInstance = nObservedTriplesPerPatternInstance;
        this->bern = bern;
        this->crossSampling = crossSampling;
        this->nWorkers = nWorkers;
    }

    TripleBatch* sample(LocalTsvCorpus<long>* corpus, INT batchSize, INT entityNegativeRate, INT relationNegativeRate, INT headBatchFlag) {
        // cout << "Creating sampler" << endl;
        GlobalSamplingState* globalState = new GlobalSamplingState(
            batchSize, entityNegativeRate, relationNegativeRate, headBatchFlag, pattern, nObservedTriplesPerPatternInstance, 500, false, false, nWorkers
        );

        pthread_t* threads = (pthread_t*) malloc(nWorkers * sizeof(pthread_t));
        DefaultLocalSamplingState<T>* localStates = (DefaultLocalSamplingState<T>*) malloc(nWorkers * sizeof(DefaultLocalSamplingState<T>));

        for (INT thread_index = 0; thread_index < nWorkers; thread_index++) {
            localStates[thread_index].id = thread_index;
            localStates[thread_index].globalState = globalState;
            localStates[thread_index].corruptionStrategy = new DefaultCorruptionStrategy<T>(corpus, thread_index);
            pthread_create(&threads[thread_index], NULL, run, (void*)(localStates + thread_index));
        }

        // cout << "Waiting for the workers to complete" << endl;

        for (INT thread_index = 0; thread_index < nWorkers; thread_index++) {
            pthread_join(threads[thread_index], NULL);
        }

        // cout << "Workers have completed" << endl;

        // TODO: free memory

        return new TripleBatch(globalState->triples, globalState->labels, globalState->nTriplesToSample);
    }

    static void* run(void* con) {
        DefaultLocalSamplingState<T>* localState = (DefaultLocalSamplingState<T> *)(con);
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

        DefaultCorruptionStrategy<T>* corruptionStrategy = localState->corruptionStrategy;

        bool bern = state->bern;
        bool crossSampling = state->crossSampling;

        INT nWorkers = state->nWorkers;

        RelationScore* relationScore = corruptionStrategy->corpus->train->relationScore;

        INT first_triple_index, last_triple_index;

        if (batchSize % nWorkers == 0) {
            first_triple_index = threadIndex * (batchSize / nWorkers);
            last_triple_index = (threadIndex + 1) * (batchSize / nWorkers);
        } else {
            first_triple_index = threadIndex * (batchSize / nWorkers + 1); // Last (incomplete) batch is distributed over all complete batches
            last_triple_index = (threadIndex + 1) * (batchSize / nWorkers + 1);
            if (last_triple_index > batchSize) last_triple_index = batchSize;  // The last batch contains fewer elements
        }

        INT patternComponentOffset = batchSize * (1 + entityNegativeRate + relationNegativeRate);

        // cout << "init run" << endl;

        for (INT batchWiseTripleIndex = first_triple_index; batchWiseTripleIndex < last_triple_index; batchWiseTripleIndex++) {
            // cout << "foo" << (*(patternInstanceSets[nObservedTriplesPerPatternInstance])).size() << endl;
            INT sampled_triple_index = rand_max(threadIndex, (*patternInstances).size());
            // cout << "Sampling " << batchWiseTripleIndex << " triple " << endl;
            PatternInstance sampledPatternInstance = (*patternInstances)[sampled_triple_index];
            INT patternComponentIndex = 0;

            // if (batchWiseTripleIndex % 10 == 0) {
            //     cout << "sampled " << batchWiseTripleIndex << " triples" << endl;
            // }

            for (Triple sampledTriple: sampledPatternInstance.triples) {
                triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex] = sampledTriple;
                labels[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex] = 1;
                // INT last = batchSize;
                // Sample negative triples
                for (INT negativeTripleOffset = 1; negativeTripleOffset <= entityNegativeRate; negativeTripleOffset++) {
                    if (!crossSampling){
                        if (bern) // flag for considering a portion of triples with unique head/tail for those of which there is a given relationship
                            headCorruptionThreshold = 1000 * relationScore->head[sampledTriple.r] / (
                                relationScore->tail[sampledTriple.r] + relationScore->head[sampledTriple.r]
                            );
                        if (randd(threadIndex) % 1000 < headCorruptionThreshold) { // Corrupt TAIL by generating a random number
                            // if (batchWiseTripleIndex == 44)
                            //     cout << "Started corrupting tail" << endl;
                            // cout << "Started corrupting tail" << endl;
                            // cout << trainList->head->left[sampledTriple.h] - 1 << "&&" << trainList->head->right[sampledTriple.h];
                            triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = corruptionStrategy->corruptTail(sampledTriple);
                            // triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = Triple(
                            //     sampledTriple.h, sampledTriple.r, corrupt_head(threadIndex, sampledTriple.h, sampledTriple.r)
                            // );
                            // if (batchWiseTripleIndex == 44)
                            //     cout << "Finished corrupting tail" << endl;
                        } else { // Corrupt HEAD
                            // if (batchWiseTripleIndex == 44)
                            //     cout << "Started corrupting head" << endl;
                            triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = corruptionStrategy->corruptHead(sampledTriple);
                            // triples[patternComponentOffset * patternComponentIndex + batchWiseTripleIndex + negativeTripleOffset * batchSize] = Triple(
                            //     corrupt_tail(threadIndex, sampledTriple.t, sampledTriple.r),  sampledTriple.r, sampledTriple.t
                            // );
                            // if (batchWiseTripleIndex == 44)
                            //     cout << "Finished corrupting head" << endl;
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
