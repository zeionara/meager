#ifndef EVALUATION_EVALUATION_SCORE_H
#define EVALUATION_EVALUATION_SCORE_H

#include <string>
#include <optional>
#include <unordered_map>

#include "../types.h"
#include "EvaluationScoreContainer.h"

#include "metric/MetricTree.h"

using namespace std;

struct EvaluationScore: EvaluationScoreContainer {

    INT value;
    MetricSetTracker* metrics;

    EvaluationScore(MetricSetTracker* metrics) {
        value = 0;
        this->metrics = metrics;
    }

    void updateMetrics();

    void updateMetrica() {
        
    }

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

    MetricTree* getTree() {
        return new MetricTree(nullptr, -1, metrics);
    }

};

#endif
