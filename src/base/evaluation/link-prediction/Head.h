#ifndef EVALUATION_LINK_PREDICTION_HEAD_H
#define EVALUATION_LINK_PREDICTION_HEAD_H

#include "Entity.h"

namespace meager::main::evaluation::link_prediction {

    namespace head {

        const string name = "head";

    }

    template <typename T>
    struct Head: Entity<T> {

        Head(corpus::Corpus* corpus, triple::list::ThickWrapper<T>* triples, triple::list::ThinWrapper<T>* testTriples, metric::score::SetMaker makeMetricSetTracker):
            Entity<T>(corpus, triples, testTriples, makeMetricSetTracker, head::name) {};

        triple::Triple makeTriple(INT i) {
            return triple::Triple(i, this->testTriples->content->items[this->currentTripleIndex].r, this->testTriples->content->items[this->currentTripleIndex].t);
        }

        INT getTripleComponent(triple::Triple triple) {
            return triple.h;
        }

    };

}

#endif
