#ifndef EVALUATION_METRIC_RANK_METRIC_TRACKER_H
#define EVALUATION_METRIC_RANK_METRIC_TRACKER_H

#include "MetricTracker.h"

struct RankMetricTracker: MetricTracker<INT> {

    // RankMetricTracker(string label): MetricTracker(label) {};
    RankMetricTracker(): MetricTracker() {};

    void update(EvaluationScore* score) {
        value += score->value + 1;
    }

};

#endif
