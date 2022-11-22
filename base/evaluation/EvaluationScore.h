#ifndef EVALUATION_EVALUATION_SCORE_H
#define EVALUATION_EVALUATION_SCORE_H

#include "../Setting.h"
#include "EvaluationScoreContainer.h"

struct MetricSetTracker;

struct EvaluationScore: EvaluationScoreContainer {

    INT value;
    MetricSetTracker* metrics;

    EvaluationScore(MetricSetTracker* metrics) {
        value = 0;
        this->metrics = metrics;
    }

    void updateMetrics();

    void resetValue() {
        value = 0;
    }

    void resetScore() {
        resetValue();
    }

    void resetMetrics();

    void reset() {
        resetValue();
        resetMetrics();
    };
};

#endif
