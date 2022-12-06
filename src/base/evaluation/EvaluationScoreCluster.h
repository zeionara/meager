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
        INT const length = 2;
        INT i = 0;

        MetricTreeNode** nodes = new MetricTreeNode*[length];

        nodes[i++] = new MetricTreeNode(UNCONSTRAINED, unconstrained->getTree());
        nodes[i++] = new MetricTreeNode(CONSTRAINED, constrained->getTree());

        return new MetricTree(nodes, length);
    }

};

#endif
