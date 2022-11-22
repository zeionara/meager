#ifndef EVALUATION_HEAD_EVALUATOR_H
#define EVALUATION_HEAD_EVALUATOR_H

#include "EntityEvaluator.h"

struct HeadEvaluator: EntityEvaluator {

    HeadEvaluator(Corpus* corpus, ThickTripleListWrapper* triples, ThinTripleListWrapper* testTriples): EntityEvaluator(corpus, triples, testTriples) {};

    Triple makeTriple(INT i) {
        return Triple(i, testTriples->content->items[currentTripleIndex].r, testTriples->content->items[currentTripleIndex].t);
    }

    INT getTripleComponent(Triple triple) {
        return triple.h;
    }

};

#endif
