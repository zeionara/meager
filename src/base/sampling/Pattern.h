#ifndef SAMPLING_PATTERN_H
#define SAMPLING_PATTERN_H

// #include "../Reader.h"
// #include "../Random.h"
// #include "../Corrupt.h"

// #include <string>

// #include "../triple/main.h"
// #include "../triple/pattern/main.h"

// #include "../corruption/LocalStrategy.h"

#include "state/local/Randomized.h"
#include "../corruption/LocalStrategy.h"
#include "state/global/Pattern.h"
// #include "state/RandomizationState.h"
#include "state/JavaLikeLcgRandomizationState.h"
#include "randomizer/LoopingRandomizer.h"
// #include "TripleBatch.h"
#include "Sampler.h"

using namespace std;

typedef void * (*PTR)(void *);

namespace meager::main::sampling {

    template <typename T, typename R>
    struct Pattern: Sampler<corpus::local::Default<INT>> {
        triple::pattern::Pattern pattern;
        INT nObservedTriplesPerPatternInstance;
        INT nWorkers;

        bool bern;
        bool crossSampling;

        Pattern(triple::pattern::Pattern pattern, INT nObservedTriplesPerPatternInstance, bool bern = false, bool crossSampling = false, INT nWorkers = 1, bool verbose = false) {
            this->pattern = pattern;
            this->nObservedTriplesPerPatternInstance = nObservedTriplesPerPatternInstance;

            this->bern = bern;
            this->crossSampling = crossSampling;

            this->nWorkers = nWorkers;

            if (verbose) {
                cout << "finished initializing pattern sampler, number of observed triples per pattern instance = " << nObservedTriplesPerPatternInstance << endl;
            }
        }

        batch::Triple* sample(corpus::local::Default<INT>* corpus, INT batchSize, INT entityNegativeRate, INT relationNegativeRate, bool headBatchFlag, bool verbose = false) {
            // if (verbose) {
            //     cout << "started initializing global state, number of observed triples per pattern instance = " << nObservedTriplesPerPatternInstance << endl;
            // }

            state::global::State* globalState = new state::global::Pattern(
                corpus->train->patterns->get(pattern),
                batchSize, entityNegativeRate, relationNegativeRate, headBatchFlag, nObservedTriplesPerPatternInstance, 500, false, false, nWorkers, verbose
            );

            // if (verbose) {
            //     cout << "finished initializing global state, number of observed triples per pattern instance = " << nObservedTriplesPerPatternInstance << endl;
            // }

            pthread_t* threads = (pthread_t*) malloc(nWorkers * sizeof(pthread_t));
            state::local::Randomized<R>* localStates = (state::local::Randomized<R>*) malloc(nWorkers * sizeof(state::local::Randomized<R>));

            for (INT thread_index = 0; thread_index < nWorkers; thread_index++) {
                localStates[thread_index].id = thread_index;
                localStates[thread_index].globalState = globalState;

                // cout << "foo" << endl;
                auto randomizer = new LoopingRandomizer<R>(new JavaLikeLcgRandomizationState<R>(rand()));
                // cout << "bar" << endl;
                localStates[thread_index].corruptionStrategy = new corruption::LocalStrategy<T, R>(corpus, thread_index, randomizer);
                localStates[thread_index].randomizer = randomizer;

                pthread_create(&threads[thread_index], NULL, run, (void*)(localStates + thread_index));
            }
            // cout << "baz --" << endl;

            for (INT thread_index = 0; thread_index < nWorkers; thread_index++) {
                pthread_join(threads[thread_index], NULL);
            }

            // cout << "Workers have completed" << endl;

            // TODO: free memory

            batch::Triple* tripleBatch = new batch::Triple(globalState->triples, globalState->labels, globalState->length);

            // cout << "Sampled triple batch" << endl;

            return tripleBatch;
        }

        static void* run(void* con) {
            state::local::Randomized<R>* localState = (state::local::Randomized<R> *)(con);
            state::global::Pattern* state = (state::global::Pattern*)localState->globalState;
            Randomizer<R>* randomizer = localState->randomizer;

            INT threadIndex = localState->id;
            triple::Triple* triples = state->triples;
            REAL* labels = state->labels;
            INT batchSize = state->batchSize;
            INT entityNegativeRate = state->entityNegativeRate;
            INT relationNegativeRate = state->relationNegativeRate;
            bool headBatchFlag = state->headBatchFlag;
            // std::vector<PatternInstance>** patternInstanceSets = para -> patternInstanceSets;
            vector<triple::pattern::Instance>* patternInstances = state->patternInstances;
            INT nObservedTriplesPerPatternInstance = state->nObservedTriplesPerPatternInstance;
            REAL headCorruptionThreshold = state->headCorruptionThreshold;

            corruption::LocalStrategy<T, R>* corruptionStrategy = (corruption::LocalStrategy<T, R>*)localState->corruptionStrategy;

            bool bern = state->bern;
            bool crossSampling = state->crossSampling;

            INT nWorkers = state->nWorkers;

            triple::relation::Score* relationScore = corruptionStrategy->corpus->train->relationScore;

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
            
            // cout << "Start sampling loop" << endl;

            for (INT batchWiseTripleIndex = first_triple_index; batchWiseTripleIndex < last_triple_index; batchWiseTripleIndex++) {
                // cout << "foo" << (*(patternInstanceSets[nObservedTriplesPerPatternInstance])).size() << endl;
                // INT sampled_triple_index = rand_max(randomizer, (*patternInstances).size());
                INT sampled_triple_index = randomizer->state->sample((*patternInstances).size());
                // cout << "Sampling " << batchWiseTripleIndex << " triple " << endl;
                triple::pattern::Instance sampledPatternInstance = (*patternInstances)[sampled_triple_index];
                INT patternComponentIndex = 0;

                // if (batchWiseTripleIndex % 10 == 0) {
                //     cout << "sampled " << batchWiseTripleIndex << " triples" << endl;
                // }

                for (triple::Triple sampledTriple: sampledPatternInstance.triples) {
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
                            // if (randd(threadIndex) % 1000 < headCorruptionThreshold) { // Corrupt TAIL by generating a random number
                            INT shouldCorruptHead = randomizer->state->sample(1000);
                            // cout << shouldCorruptHead << endl;
                            if (shouldCorruptHead < headCorruptionThreshold) { // Corrupt TAIL by generating a random number
                            // if (randomizer->state->sample(1000) < 2000) { // Corrupt TAIL by generating a random number
                                // cout << "+" << endl;
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
                                // cout << "-" << endl;
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

                    INT positiveTripleIndex         = patternComponentOffset * observedTripleIndex                                  + batchWiseTripleIndex;
                    INT positiveObservedTripleIndex = patternComponentOffset * (patternComponentIndex + observedTripleIndexCounter) + batchWiseTripleIndex;

                    triples[positiveObservedTripleIndex] = triples[positiveTripleIndex];
                    labels [positiveObservedTripleIndex] = 1;
                    // labels[patternComponentOffset * observedTripleIndex + batchWiseTripleIndex];

                    // INT last = batchSize;

                    for (INT negativeTripleOffset = 1; negativeTripleOffset <= entityNegativeRate + relationNegativeRate; negativeTripleOffset++) {

                        INT negativeTripleIndex         = positiveTripleIndex         + negativeTripleOffset * batchSize;
                        INT negativeObservedTripleIndex = positiveObservedTripleIndex + negativeTripleOffset * batchSize;

                        triples[negativeObservedTripleIndex] = triples[negativeTripleIndex];
                        labels [negativeObservedTripleIndex] = -1;
                        // last += batchSize;
                    }

                    observedTripleIndexCounter++;
                }
            } // end loop for each positive triple in the batch which should be processed by the thread
            // cout << "Stop sampling loop" << endl;
            pthread_exit(NULL);
        }
    };

}

#endif
