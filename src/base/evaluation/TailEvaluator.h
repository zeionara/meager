#ifndef EVALUATION_TAIL_EVALUATOR_H
#define EVALUATION_TAIL_EVALUATOR_H

#include "EntityEvaluator.h"

template <typename T>
struct TailEvaluator: EntityEvaluator<T> {

    TailEvaluator(Corpus* corpus, triple::ThickTripleListWrapper<T>* triples, triple::ThinTripleListWrapper<T>* testTriples, MetricSetTrackerMaker makeMetricSetTracker):
        EntityEvaluator<T>(corpus, triples, testTriples, makeMetricSetTracker, "tail") {};

    triple::Triple makeTriple(INT i) {
        return triple::Triple(this->testTriples->content->items[this->currentTripleIndex].h, this->testTriples->content->items[this->currentTripleIndex].r, i);
    }

    INT getTripleComponent(triple::Triple triple) {
        return triple.t;
    }

};

#endif
