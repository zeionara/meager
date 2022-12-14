#ifndef EVALUATION_LINK_PREDICTION_EVALUATOR_H
#define EVALUATION_LINK_PREDICTION_EVALUATOR_H

#include <functional>

#include "Head.h"
#include "Tail.h"

namespace meager::main::evaluation::link_prediction {

    template <typename T>
    struct Evaluator {

        corpus::Corpus* corpus;

        triple::list::ThickWrapper<T>* triples;
        triple::list::ThinWrapper<T>* testTriples;

        Head<T>* head;
        Tail<T>* tail;

        Evaluator(corpus::Corpus* corpus, triple::list::ThickWrapper<T>* triples, triple::list::ThinWrapper<T>* testTriples, metric::score::SetMaker makeMetricSetTracker) {
            this->corpus = corpus;
            this->triples = triples;
            this->testTriples = testTriples;

            // cout << "Making evaluators" << endl;
            this->head = new Head<T>(corpus, triples, testTriples, makeMetricSetTracker);
            this->tail = new Tail<T>(corpus, triples, testTriples, makeMetricSetTracker);
            // cout << "Made evaluators" << endl;
        }

        void printMetrics() {
            // this->head->state->printMetrics("head", this->testTriples->length);
            this->head->state->unconstrained->unfiltered->metrics->printHeader();
            this->head->state->printMetrics(this->testTriples->length);
            this->tail->state->printMetrics(this->testTriples->length);
        }

        metric::tree::Root* getMetricTree() {
            INT const length = 2;
            INT i = 0;

            metric::tree::Node** nodes = new metric::tree::Node*[length];

            nodes[i++] = new metric::tree::Node(head::name, head->state->getTree());
            nodes[i++] = new metric::tree::Node(tail::name, tail->state->getTree());

            return new metric::tree::Root(new metric::tree::Tree(nodes, length), testTriples->length);
        }

    };

}

#endif
