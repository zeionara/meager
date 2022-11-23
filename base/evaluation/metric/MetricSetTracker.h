#ifndef EVALUATION_METRIC_METRIC_SET_TRACKER
#define EVALUATION_METRIC_METRIC_SET_TRACKER

#include <iostream>

#include "MetricTracker.h"

#include "CountMetricTracker.h"
#include "RankMetricTracker.h"
#include "ReciprocalRankMetricTracker.h"

using namespace std;

struct MetricSetTracker {
    MetricTrackerBase** trackers;
    INT length;

    MetricSetTracker(MetricTrackerBase** trackers, INT length) {
    // MetricSetTracker() {
        this->trackers = trackers;
        this->length = length;
        // trackers = (MetricTrackerBase**) malloc(5 * sizeof(MetricTrackerBase*));

        // trackers[0] = new CountMetricTracker(1);
        // trackers[1] = new CountMetricTracker(3);
        // trackers[2] = new CountMetricTracker(10);
        // trackers[3] = new RankMetricTracker();
        // trackers[4] = new ReciprocalRankMetricTracker();

        // length = 5;
    }

    void update(EvaluationScore* score) {
        // cout << "Updating metrics one by one" << endl;
        // cout << score << endl;
        // cout << length << endl;
        for (INT i = 0; i < length; i++) {
            // cout << "Updating " << i << endl;
            // cout << this->trackers[i] << endl;
            this->trackers[i]->update(score);
        }
    }

    void reset() {
        for (INT i = 0; i < length; i++) {
            this->trackers[i]->reset();
        }
    }

    void printHeader() {
        printf("metric");

        for (INT i = 0; i < length; i++) {
            printf("\t%s", trackers[i]->getLabel().c_str());
        }

        printf("\n");
    }

    void printMetrics(string prefix, INT nTriples) {
        // string formatString = "%s";

        printf("%s", prefix.c_str());

        for (INT i = 0; i < length; i++) {
            printf("\t%f", trackers[i]->divide(nTriples));
        }

        printf("\n");

        // printf(
        //     "%s\t%f\t%f\t%f\t%f\t%f\n",
        //     // formatString.c_str(),
        //     prefix.c_str(), trackers[0]->divide(nTriples), trackers[1]->divide(nTriples), trackers[2]->divide(nTriples), trackers[3]->divide(nTriples), trackers[4]->divide(nTriples)
        // );
    }
};

#endif
