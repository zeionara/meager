#ifndef EVALUATION_EVALUATION_SCORE_GROUP
#define EVALUATION_EVALUATION_SCORE_GROUP

#include <functional>
#include <unordered_map>

#include "EvaluationScore.h"
#include "EvaluationScoreContainer.h"

#include "metric/MetricSetTracker.h"

typedef function<MetricSetTracker*(string)> MetricSetTrackerMaker;

string const UNFILTERED = "unfiltered";
string const FILTERED = "filtered";
string const SEPARATOR = "_";

struct EvaluationScoreGroup: EvaluationScoreContainer {

    EvaluationScore* filtered;
    EvaluationScore* unfiltered;

    string label;

    EvaluationScoreGroup(MetricSetTrackerMaker makeMetricSetTracker, string label) {
        unfiltered = new EvaluationScore(makeMetricSetTracker(label + SEPARATOR + UNFILTERED));
        filtered = new EvaluationScore(makeMetricSetTracker(label + SEPARATOR + FILTERED));
        this->label = label;
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

    void printMetrics(INT nTriples) {
        unfiltered->metrics->printMetrics(label + SEPARATOR + UNFILTERED, nTriples);
        filtered->metrics->printMetrics(label + SEPARATOR + FILTERED, nTriples);
    }

    MetricTree* getTree() {
        unordered_map<string, MetricTree*> subtrees = {{UNFILTERED, unfiltered->getTree()}, {FILTERED, filtered->getTree()}};
        return new MetricTree(subtrees);
    }

};

#endif
