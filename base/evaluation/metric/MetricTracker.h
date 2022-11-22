#ifndef EVALUATION_METRIC_METRIC_TRACKER_H
#define EVALUATION_METRIC_METRIC_TRACKER_H

#include "../EvaluationScore.h"

struct MetricTrackerBase {
    virtual void update(EvaluationScore* score) = 0;
    virtual void reset() = 0;
    virtual REAL divide(INT divisor) = 0;
};

template<typename T> struct MetricTracker: MetricTrackerBase {

    T value;

    MetricTracker() {
        value = 0;
    }

    void reset() {
        value = 0;
    }

    REAL divide(INT divisor) {
        return value / divisor;
    }

};



#endif
