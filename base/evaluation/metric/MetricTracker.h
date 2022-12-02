#ifndef EVALUATION_METRIC_METRIC_TRACKER_H
#define EVALUATION_METRIC_METRIC_TRACKER_H

#include <string>

#include "../EvaluationScore.h"
#include "Metric.h"

using namespace std;

struct MetricTrackerBase {
    // string label;

    virtual void update(EvaluationScore* score) = 0;
    virtual void reset() = 0;
    virtual string getLabel() = 0;
    virtual string getName() = 0;
    virtual double divide(INT divisor) = 0;
    virtual Metric getId() = 0;
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

    double divide(INT divisor) {
        return double(value) / divisor;
    }

};



#endif
