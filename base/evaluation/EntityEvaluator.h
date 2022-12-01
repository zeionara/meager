#ifndef EVALUATION_ENTITY_EVALUATOR_H
#define EVALUATION_ENTITY_EVALUATOR_H

#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
#include "../samplers/TripleBatch.h"
#include "../corruption/CorruptionStrategy.h"
#include "EvaluationScoreCluster.h"
#include "../storage/Corpus.h"

template <typename T>
struct EntityEvaluator {
    INT currentTripleIndex;

    EvaluationScoreCluster* state;
    Corpus* corpus;

    ThickTripleListWrapper<T>* triples;
    ThinTripleListWrapper<T>* testTriples;

    // virtual bool isTripleFromDataset(Triple triple) = 0;
    // virtual bool isCorrectTriple(Triple triple) = 0;
   
    EntityEvaluator(Corpus* corpus, ThickTripleListWrapper<T>* triples, ThinTripleListWrapper<T>* testTriples, MetricSetTrackerMaker makeMetricSetTracker) {
        this->corpus = corpus;
        this->triples = triples;
        this->testTriples = testTriples;
        this->state = new EvaluationScoreCluster(makeMetricSetTracker);
    }

    TripleBatch* makeBatch() { // Generate all possible triples for every entity which may be used as a triple head
        Triple* triples = (Triple*) malloc(this->corpus->countEntities() * sizeof(Triple));

        for (INT i = 0; i < this->corpus->countEntities(); i++) {
            triples[i] = makeTriple(i);
        }

        return new TripleBatch(triples, this->corpus->countEntities());
    }

    void evaluate(REAL *probabilities, bool reverse, bool verbose = false) {
        // cout << "Test triples @ " << currentTripleIndex << endl;
        // cout << testTriples << endl;

        Triple reference = testTriples->content->items[currentTripleIndex];

        // cout << "Reference @ " << getTripleComponent(reference) <<  endl;

        REAL reference_distance = probabilities[getTripleComponent(reference)];

        // cout << state->unconstrained->unfiltered->value;
        state->resetScore();
        //

        for (INT hypothesis = 0; hypothesis < this->corpus->countEntities(); hypothesis++) {
            if (hypothesis != getTripleComponent(reference)) { 

                Triple sampledTriple = makeTriple(hypothesis);

                REAL hypothesis_distance = probabilities[hypothesis];
                if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                    state->unconstrained->unfiltered->value += 1; // Count incorrectly classified triples
                    if (not corpus->contains(sampledTriple)) // If less probable triple is not present in the dataset
                        state->unconstrained->filtered->value += 1; // Count incorrectly classified triples which are not present in the dataset (filtered score must be better than unfiltered concerning rank)
                }

                if (corpus->allows(sampledTriple)) {
                    if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                        state->constrained->unfiltered->value += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads (constrained score must be better than unconstrained)
                        if (not corpus->contains(sampledTriple)) {
                            state->constrained->filtered->value += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads but triple does not exist
                        }
                    }
                }

            }
        }

        // cout << "Updating metrics" << endl;

        // cout << state << endl;

        state->updateMetrics();

        // cout << "Updated metrics" << endl;

        currentTripleIndex++;
    }

    private:

    virtual Triple makeTriple(INT i) = 0;
    virtual INT getTripleComponent(Triple triple) = 0;

};

#endif
