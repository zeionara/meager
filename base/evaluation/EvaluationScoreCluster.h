#ifndef EVALUATION_EVALUATION_SCORE_CLUSTER
#define EVALUATION_EVALUATION_SCORE_CLUSTER

#include "EvaluationScoreGroup.h"
#include "EvaluationScoreContainer.h"

string const CONSTRAINED = "constrained";
string const UNCONSTRAINED = "unconstrained";

struct EvaluationScoreCluster: EvaluationScoreContainer {

    EvaluationScoreGroup* constrained;
    EvaluationScoreGroup* unconstrained;

    string label;

    EvaluationScoreCluster(MetricSetTrackerMaker makeMetricSetTracker, string label) {
        unconstrained = new EvaluationScoreGroup(makeMetricSetTracker, label + SEPARATOR + UNCONSTRAINED);
        constrained = new EvaluationScoreGroup(makeMetricSetTracker, label + SEPARATOR + CONSTRAINED);
        this->label = label;
    }

    void updateMetrics() {
        // cout << "Updating metrics in cluster" << endl;
        constrained->updateMetrics();
        // cout << "Updating metrics in cluster" << endl;
        unconstrained->updateMetrics();
    }

    void resetScore() {
        constrained->resetScore();
        unconstrained->resetScore();
    }

    void resetMetrics() {
        constrained->resetMetrics();
        unconstrained->resetMetrics();
    }

    void reset() {
        constrained->reset();
        unconstrained->reset();
    }

    void printMetrics(INT nTriples) {
        unconstrained->printMetrics(nTriples);
        constrained->printMetrics(nTriples);
    }

    MetricTree* getTree() {
        unordered_map<string, MetricTree*> subtrees = {{UNCONSTRAINED, unconstrained->getTree()}, {CONSTRAINED, constrained->getTree()}};
        return new MetricTree(subtrees);
    }

};

#endif
