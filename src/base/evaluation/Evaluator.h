#ifndef EVALUATION_EVALUATOR_H
#define EVALUATION_EVALUATOR_H

#include <functional>

#include "../triple/list/ThickWrapper.h"
#include "../triple/list/ThinWrapper.h"
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

    triple::list::ThickWrapper<T>* triples;
    triple::list::ThinWrapper<T>* testTriples;

    HeadEvaluator<T>* head;
    TailEvaluator<T>* tail;

    Evaluator(Corpus* corpus, triple::list::ThickWrapper<T>* triples, triple::list::ThinWrapper<T>* testTriples, MetricSetTrackerMaker makeMetricSetTracker) {
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

    MetricTreeRoot* getMetricTree() {
        INT const length = 2;
        INT i = 0;

        MetricTreeNode** nodes = new MetricTreeNode*[length];

        nodes[i++] = new MetricTreeNode(HEAD, head->state->getTree());
        nodes[i++] = new MetricTreeNode(TAIL, tail->state->getTree());

        return new MetricTreeRoot(new MetricTree(nodes, length), testTriples->length);
    }

};

#endif
