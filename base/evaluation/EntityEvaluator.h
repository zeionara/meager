#ifndef EVALUATION_ENTITY_EVALUATOR_H
#define EVALUATION_ENTITY_EVALUATOR_H

#include "../Setting.h"
#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
#include "../samplers/TripleBatch.h"
#include "../corruption/CorruptionStrategy.h"
#include "EvaluationScoreCluster.h"
#include "../storage/Corpus.h"

struct EntityEvaluator {
    INT currentTripleIndex;

    EvaluationScoreCluster* state;
    Corpus* corpus;

    ThickTripleListWrapper* triples;
    ThinTripleListWrapper* testTriples;

    // virtual bool isTripleFromDataset(Triple triple) = 0;
    // virtual bool isCorrectTriple(Triple triple) = 0;
   
    EntityEvaluator(Corpus* corpus, ThickTripleListWrapper* triples, ThinTripleListWrapper* testTriples) {
        this->corpus = corpus;
        this->triples = triples;
        this->testTriples = testTriples;
        this->state = new EvaluationScoreCluster();
    }

    TripleBatch makeBatch() { // Generate all possible triples for every entity which may be used as a triple head
        Triple* triples = (Triple*) malloc(this->triples->frequencies->nEntities * sizeof(Triple));

        for (INT i = 0; i < this->triples->frequencies->nEntities; i++) {
            triples[i] = makeTriple(i);
        }

        return TripleBatch(triples, this->triples->frequencies->nEntities);
    }

    void evaluate(REAL *probabilities, bool reverse) {
        Triple reference = testTriples->content->items[currentTripleIndex];

        REAL reference_distance = probabilities[getTripleComponent(reference)];

        state->resetScore();

        for (INT hypothesis = 0; hypothesis < triples->frequencies->nEntities; hypothesis++) {
            if (hypothesis != getTripleComponent(reference)) { 

                Triple sampledTriple = makeTriple(hypothesis);

                REAL hypothesis_distance = probabilities[hypothesis];
                if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                    state->unconstrained->unfiltered += 1; // Count incorrectly classified triples
                    if (not corpus->contains(sampledTriple)) // If less probable triple is not present in the dataset
                        state->unconstrained->filtered += 1; // Count incorrectly classified triples which are not present in the dataset (filtered score must be better than unfiltered concerning rank)
                }

                if (corpus->allows(sampledTriple)) {
                    if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                        state->constrained->unfiltered += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads (constrained score must be better than unconstrained)
                        if (not corpus->contains(sampledTriple)) {
                            state->constrained->filtered += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads but triple does not exist
                        }
                    }
                }

            }
        }

        state->updateMetrics();
        currentTripleIndex++;
    }

    private:

    virtual Triple makeTriple(INT i) = 0;
    virtual INT getTripleComponent(Triple triple) = 0;

};

#endif
