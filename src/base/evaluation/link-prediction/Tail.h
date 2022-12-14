#ifndef EVALUATION_LINK_PREDICTION_TAIL_H
#define EVALUATION_LINK_PREDICTION_TAIL_H

#include "Entity.h"

namespace meager::main::evaluation::link_prediction {

    namespace tail {

        const string name = "tail";

    }

    template <typename T>
    struct Tail: Entity<T> {

        Tail(corpus::Corpus* corpus, triple::list::ThickWrapper<T>* triples, triple::list::ThinWrapper<T>* testTriples, metric::score::SetMaker makeMetricSetTracker):
            Entity<T>(corpus, triples, testTriples, makeMetricSetTracker, tail::name) {};

        triple::Triple makeTriple(INT i) {
            return triple::Triple(this->testTriples->content->items[this->currentTripleIndex].h, this->testTriples->content->items[this->currentTripleIndex].r, i);
        }

        INT getTripleComponent(triple::Triple triple) {
            return triple.t;
        }

    };

}

#endif
