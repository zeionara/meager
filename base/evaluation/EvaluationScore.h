#ifndef EVALUATION_EVALUATION_SCORE_H
#define EVALUATION_EVALUATION_SCORE_H

#include <string>
#include <optional>
#include <unordered_map>

#include "../types.h"
#include "EvaluationScoreContainer.h"

using namespace std;

struct MetricSetTracker;

struct MetricTree {
    optional<unordered_map<string, MetricTree*>> children;
    optional<MetricSetTracker*> metrics;

    MetricTree(optional<unordered_map<string, MetricTree*>> children, optional<MetricSetTracker*> metrics = {}) {
        this->children = children;
        this->metrics = metrics;
    }
};

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
        return new MetricTree({}, optional<MetricSetTracker*>{metrics});
    }

};

#endif
