#ifndef EVALUATION_TAIL_EVALUATOR_H
#define EVALUATION_TAIL_EVALUATOR_H

#include "EntityEvaluator.h"

struct TailEvaluator: EntityEvaluator {

    TailEvaluator(Corpus* corpus, ThickTripleListWrapper* triples, ThinTripleListWrapper* testTriples): EntityEvaluator(corpus, triples, testTriples) {};

    Triple makeTriple(INT i) {
        return Triple(testTriples->content->items[currentTripleIndex].t, testTriples->content->items[currentTripleIndex].r, i);
    }

    INT getTripleComponent(Triple triple) {
        return triple.t;
    }

};

#endif
