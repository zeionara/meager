#ifndef EVALUATION_METRIC_METRIC_SET_TRACKER
#define EVALUATION_METRIC_METRIC_SET_TRACKER

#include "MetricTracker.h"

#include "CountMetricTracker.h"
#include "RankMetricTracker.h"
#include "ReciprocalRankMetricTracker.h"

using namespace std;

struct MetricSetTracker {
    MetricTrackerBase* trackers;
    INT length;

    MetricSetTracker() {
        MetricTrackerBase* trackers[5];

        trackers[0] = new CountMetricTracker(1);
        trackers[1] = new CountMetricTracker(3);
        trackers[2] = new CountMetricTracker(10);
        trackers[3] = new RankMetricTracker();
        trackers[4] = new ReciprocalRankMetricTracker();

        length = 5;
    }

    void update(EvaluationScore* score) {
        for (INT i = 0; i < length; i++) {
            this->trackers[i].update(score);
        }
    }

    void reset() {
        for (INT i = 0; i < length; i++) {
            this->trackers[i].reset();
        }
    }

    void printMetrics(string prefix, INT nTriples) {
        printf(
            "%s\t%f\t%f\t%f\t%f\t%f\n",
            prefix.c_str(), trackers[0].divide(nTriples), trackers[1].divide(nTriples), trackers[2].divide(nTriples), trackers[3].divide(nTriples), trackers[4].divide(nTriples)
        );
    }
};

#endif
