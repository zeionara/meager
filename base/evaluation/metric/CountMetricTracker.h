#ifndef EVALUATION_METRIC_COUNT_METRIC_TRACKER_H
#define EVALUATION_METRIC_COUNT_METRIC_TRACKER_H

#include "MetricTracker.h"

struct CountMetricTracker: MetricTracker<INT> {

    INT n;

    CountMetricTracker(INT n): MetricTracker() {
        this->n = n;
    }

    void update(EvaluationScore* score) {
        if (score->value < n) {
            value += 1;
        }
    }

};

#endif
