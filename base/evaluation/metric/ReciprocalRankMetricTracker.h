#ifndef EVALUATION_METRIC_RECIPROCAL_RANK_METRIC_TRACKER
#define EVALUATION_METRIC_RECIPROCAL_RANK_METRIC_TRACKER

#include "MetricTracker.h"

string const RECIPROCAL_RANK = "reciprocal_rank";

struct ReciprocalRankMetricTracker: MetricTracker<REAL> {

    /// ReciprocalRankMetricTracker(string label): MetricTracker(label) {};
    ReciprocalRankMetricTracker(): MetricTracker() {};
    // ReciprocalRankMetricTracker(): MetricTracker() {};

    void update(EvaluationScore* score) {
        value += 1.0 / (score->value + 1);
    }

    string getLabel() {
        return RECIPROCAL_RANK;
    }

    string getName() {
        return RECIPROCAL_RANK;
    }

    Metric getId() {
        return ReciprocalRank;
    }
};

#endif
