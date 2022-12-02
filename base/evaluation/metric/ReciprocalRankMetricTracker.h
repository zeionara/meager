#ifndef EVALUATION_METRIC_RECIPROCAL_RANK_METRIC_TRACKER
#define EVALUATION_METRIC_RECIPROCAL_RANK_METRIC_TRACKER

#include "MetricTracker.h"

struct ReciprocalRankMetricTracker: MetricTracker<REAL> {

    /// ReciprocalRankMetricTracker(string label): MetricTracker(label) {};
    ReciprocalRankMetricTracker(): MetricTracker() {};
    // ReciprocalRankMetricTracker(): MetricTracker() {};

    void update(EvaluationScore* score) {
        value += 1.0 / (score->value + 1);
    }

    string getLabel() {
        return "reciprocal-rank";
    }

    string getName() {
        return "reciprocal_rank";
    }

    Metric getId() {
        return ReciprocalRank;
    }
};

#endif
