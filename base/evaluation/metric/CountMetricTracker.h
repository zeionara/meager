#ifndef EVALUATION_METRIC_COUNT_METRIC_TRACKER_H
#define EVALUATION_METRIC_COUNT_METRIC_TRACKER_H

#include <iostream>

#include "MetricTracker.h"

using namespace std;

string const COUNT = "top_n";

struct CountMetricTracker: MetricTracker<INT> {

    INT n;

    CountMetricTracker(INT n): MetricTracker() {
    // CountMetricTracker(INT n, string label): MetricTracker(label) {
    // CountMetricTracker(INT n): MetricTracker() {
        this->n = n;
    }

    void update(EvaluationScore* score) {
        // cout << "Update count metric score" << endl;
        // cout << "Got score " << score->value << endl;
        if (score->value < n) {
            value += 1;
        }
    }

    string getLabel() {
        return COUNT + "@" + to_string(n);
    }

    string getName() {
        return COUNT;
    }

    Metric getId() {
        return Count;
    }
};

#endif
