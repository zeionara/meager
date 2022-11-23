#ifndef EVALUATION_METRIC_METRIC_TRACKER_H
#define EVALUATION_METRIC_METRIC_TRACKER_H

#include "../EvaluationScore.h"

using namespace std;

struct MetricTrackerBase {
    // string label;

    virtual void update(EvaluationScore* score) = 0;
    virtual void reset() = 0;
    virtual string getLabel() = 0;
    virtual REAL divide(INT divisor) = 0;
};

template<typename T> struct MetricTracker: MetricTrackerBase {

    T value;

    // MetricTracker(string label = "none") {
    MetricTracker() {
        value = 0;
        // this->label = label;
    }

    void reset() {
        value = 0;
    }

    REAL divide(INT divisor) {
        return double(value) / divisor;
    }

};



#endif
