#ifndef EVALUATION_EVALUATOR_H
#define EVALUATION_EVALUATOR_H

#include <functional>

#include "../Setting.h"
#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
#include "../samplers/TripleBatch.h"
#include "../corruption/CorruptionStrategy.h"

#include "EvaluationScoreCluster.h"
#include "../storage/Corpus.h"
#include "HeadEvaluator.h"
#include "TailEvaluator.h"

struct Evaluator {

    Corpus* corpus;

    ThickTripleListWrapper* triples;
    ThinTripleListWrapper* testTriples;

    HeadEvaluator* head;
    TailEvaluator* tail;

    Evaluator(Corpus* corpus, ThickTripleListWrapper* triples, ThinTripleListWrapper* testTriples, MetricSetTrackerMaker makeMetricSetTracker) {
        this->corpus = corpus;
        this->triples = triples;
        this->testTriples = testTriples;

        this->head = new HeadEvaluator(corpus, triples, testTriples, makeMetricSetTracker);
        this->tail = new TailEvaluator(corpus, triples, testTriples, makeMetricSetTracker);
    }

    void printMetrics() {
        // this->head->state->printMetrics("head", this->testTriples->length);
        // this->head->state->unconstrained->unfiltered->metrics->printHeader();
        this->head->state->printMetrics("head", this->testTriples->length);
        this->tail->state->printMetrics("tail", this->testTriples->length);
    }
};

// struct EvaluationState {
//     INT lastHead;
//     INT lastTail;
// 
//     EvaluationScoreGroup* constrained;
//     EvaluationScoreGroup* unconstrained;
// 
//     EvaluationState() {
//         lastHead = 0;
//         lastTail = 0;
// 
//         constrained = new EvaluationScoreGroup();
//         unconstrained = new EvaluationScoreGroup();
//     }
// 
//     void resetScore() {
//         constrained->reset();
//         unconstrained->reset();
//     }
// 
//     void updateMetrics() {
//         constrained->updateMetrics();
//         unconstrained->updateMetrics();
//     }
// };
// 
// struct Evaluator {
//     EvaluationState* state;
// 
//     ThickTripleListWrapper* triples;
//     ThinTripleListWrapper* testTriples;
// 
//     CorruptionStrategy* corruptionStrategy;
// 
//     TripleBatch getHeadBatch() {
//         return getBatch(
//             [&](INT i){return Triple(i, testTriples->content->items[state->lastHead].r, testTriples->content->items[state->lastHead].t);}
//         );
//     }
// 
//     TripleBatch getTailBatch() {
//         return getBatch(
//             [&](INT i){return Triple(testTriples->content->items[state->lastHead].h, testTriples->content->items[state->lastHead].r, i);}
//         );
//     }
// 
// 
//     void evaluate(
//         REAL *probabilities, bool reverse, INT lastTripleComponent, INT (*getTripleComponent)(Triple triple), Triple (*makeTriple)(INT hypothesis, Triple triple),
//         function<bool(Triple)> isTripleFromDataset, function<bool(Triple)> isCorrectTriple
//     ) {
//         Triple reference = testTriples->content->items[lastTripleComponent];
// 
//         REAL reference_distance = probabilities[getTripleComponent(reference)];
// 
//         state->resetScore();
// 
//         for (INT hypothesis = 0; hypothesis < triples->frequencies->nEntities; hypothesis++) {
//             if (hypothesis != getTripleComponent(reference)) { 
//                 Triple sampledTriple = makeTriple(hypothesis, reference);
//                 REAL hypothesis_distance = probabilities[hypothesis];
//                 if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
//                     state->unconstrained->unfiltered += 1; // Count incorrectly classified triples
//                     if (not isTripleFromDataset(sampledTriple)) // If less probable triple is not present in the dataset
//                         state->unconstrained->filtered += 1; // Count incorrectly classified triples which are not present in the dataset (filtered score must be better than unfiltered concerning rank)
//                 }
// 
//                 if (isCorrectTriple(sampledTriple)) {
//                     if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
//                         state->constrained->unfiltered += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads (constrained score must be better than unconstrained)
//                         if (not isTripleFromDataset(sampledTriple)) {
//                             state->constrained->filtered += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads but triple does not exist
//                         }
//                     }
//                 }
//             }
//         }
// 
//         state->updateMetrics();
//         state->lastHead++;
//     }
// 
//     private: TripleBatch getBatch(function<Triple(INT)> makeTriple) { // Generate all possible triples for every entity which may be used as a triple head
//         Triple* triples = (Triple*) malloc(this->triples->frequencies->nEntities * sizeof(Triple));
// 
//         for (INT i = 0; i < this->triples->frequencies->nEntities; i++) {
//             triples[i] = makeTriple(i);
//         }
// 
//         return TripleBatch(triples, this->triples->frequencies->nEntities);
//     }
// 
// };

#endif
