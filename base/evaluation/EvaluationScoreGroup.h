#ifndef EVALUATION_EVALUATION_SCORE_GROUP
#define EVALUATION_EVALUATION_SCORE_GROUP

#include "EvaluationScore.h"
#include "EvaluationScoreContainer.h"

#include "metric/MetricSetTracker.h"

struct EvaluationScoreGroup: EvaluationScoreContainer {

    EvaluationScore* filtered;
    EvaluationScore* unfiltered;

    EvaluationScoreGroup() {
        filtered = new EvaluationScore(new MetricSetTracker());
        unfiltered = new EvaluationScore(new MetricSetTracker());
    }

    void updateMetrics() {
        filtered->updateMetrics();
        unfiltered->updateMetrics();
    }

    void resetScore() {
        filtered->resetValue();
        unfiltered->resetValue();
    }

    void resetMetrics() {
        filtered->resetMetrics();
        unfiltered->resetMetrics();
    }

    void reset() {
        filtered->reset();
        unfiltered->reset();
    }

    void printMetrics(string prefix, INT nTriples) {
        filtered->metrics->printMetrics(prefix + " filtered", nTriples);
        filtered->metrics->printMetrics(prefix + " unfiltered", nTriples);
    }

};

#endif
