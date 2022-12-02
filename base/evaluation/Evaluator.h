#ifndef EVALUATION_EVALUATOR_H
#define EVALUATION_EVALUATOR_H

#include <functional>

#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
#include "../samplers/TripleBatch.h"
#include "../corruption/CorruptionStrategy.h"

#include "EvaluationScoreCluster.h"
#include "../storage/Corpus.h"
#include "HeadEvaluator.h"
#include "TailEvaluator.h"

// struct MetricTree {
//     unordered_map<string, MetricTree*> children;
//     MetricSetTracker* metrics;
// 
//     MetricTree(unordered_map<string, MetricTree*> children, MetricSetTracker* metrics) {
//         this->children = children;
//         this->metrics = metrics;
//     }
// };

string const HEAD = "head";
string const TAIL = "tail";

template <typename T>
struct Evaluator {

    Corpus* corpus;

    ThickTripleListWrapper<T>* triples;
    ThinTripleListWrapper<T>* testTriples;

    HeadEvaluator<T>* head;
    TailEvaluator<T>* tail;

    Evaluator(Corpus* corpus, ThickTripleListWrapper<T>* triples, ThinTripleListWrapper<T>* testTriples, MetricSetTrackerMaker makeMetricSetTracker) {
        this->corpus = corpus;
        this->triples = triples;
        this->testTriples = testTriples;

        // cout << "Making evaluators" << endl;
        this->head = new HeadEvaluator<T>(corpus, triples, testTriples, makeMetricSetTracker);
        this->tail = new TailEvaluator<T>(corpus, triples, testTriples, makeMetricSetTracker);
        // cout << "Made evaluators" << endl;
    }

    void printMetrics() {
        // this->head->state->printMetrics("head", this->testTriples->length);
        this->head->state->unconstrained->unfiltered->metrics->printHeader();
        this->head->state->printMetrics(this->testTriples->length);
        this->tail->state->printMetrics(this->testTriples->length);
    }

    MetricTree* getMetricTree() {
        unordered_map<string, MetricTree*> subtrees = {{HEAD, head->state->getTree()}, {TAIL, tail->state->getTree()}};
        return new MetricTree(subtrees);
    }

};

#endif
