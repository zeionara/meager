#ifndef EVALUATION_TAIL_EVALUATOR_H
#define EVALUATION_TAIL_EVALUATOR_H

#include "EntityEvaluator.h"

template <typename T>
struct TailEvaluator: EntityEvaluator<T> {

    TailEvaluator(Corpus* corpus, ThickTripleListWrapper<T>* triples, ThinTripleListWrapper<T>* testTriples, MetricSetTrackerMaker makeMetricSetTracker):
        EntityEvaluator<T>(corpus, triples, testTriples, makeMetricSetTracker, "tail") {};

    Triple makeTriple(INT i) {
        return Triple(this->testTriples->content->items[this->currentTripleIndex].t, this->testTriples->content->items[this->currentTripleIndex].r, i);
    }

    INT getTripleComponent(Triple triple) {
        return triple.t;
    }

};

#endif
