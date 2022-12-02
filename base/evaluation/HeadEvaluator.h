#ifndef EVALUATION_HEAD_EVALUATOR_H
#define EVALUATION_HEAD_EVALUATOR_H

#include "EntityEvaluator.h"

template <typename T>
struct HeadEvaluator: EntityEvaluator<T> {

    HeadEvaluator(Corpus* corpus, ThickTripleListWrapper<T>* triples, ThinTripleListWrapper<T>* testTriples, MetricSetTrackerMaker makeMetricSetTracker):
        EntityEvaluator<T>(corpus, triples, testTriples, makeMetricSetTracker, "head") {};

    Triple makeTriple(INT i) {
        return Triple(i, this->testTriples->content->items[this->currentTripleIndex].r, this->testTriples->content->items[this->currentTripleIndex].t);
    }

    INT getTripleComponent(Triple triple) {
        return triple.h;
    }

};

#endif
