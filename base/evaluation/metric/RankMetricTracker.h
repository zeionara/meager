#ifndef EVALUATION_METRIC_RANK_METRIC_TRACKER_H
#define EVALUATION_METRIC_RANK_METRIC_TRACKER_H

#include "MetricTracker.h"

string const RANK = "rank";

struct RankMetricTracker: MetricTracker<INT> {

    RankMetricTracker(): MetricTracker() {};
    // RankMetricTracker(string label): MetricTracker(label) {};
    // RankMetricTracker(): MetricTracker() {};

    void update(EvaluationScore* score) {
        value += score->value + 1;
    }

    string getLabel() {
        return RANK;
    }

    string getName() {
        return RANK;
    }

    Metric getId() {
        return Rank;
    }
};

#endif
