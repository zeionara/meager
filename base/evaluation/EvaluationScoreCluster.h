#ifndef EVALUATION_EVALUATION_SCORE_CLUSTER
#define EVALUATION_EVALUATION_SCORE_CLUSTER

#include "EvaluationScoreGroup.h"
#include "EvaluationScoreContainer.h"

struct EvaluationScoreCluster: EvaluationScoreContainer {

    EvaluationScoreGroup* constrained;
    EvaluationScoreGroup* unconstrained;

    EvaluationScoreCluster() {
        constrained = new EvaluationScoreGroup();
        unconstrained = new EvaluationScoreGroup();
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

    void printMetrics(string prefix, INT nTriples) {
        constrained->printMetrics(prefix + " constrained", nTriples);
        unconstrained->printMetrics(prefix + " unconstrained", nTriples);
    }

};

#endif
