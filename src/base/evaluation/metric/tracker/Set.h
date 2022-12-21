#ifndef EVALUATION_METRIC_TRACKER_SET_H
#define EVALUATION_METRIC_TRACKER_SET_H

#include <iostream>

#include "Tracker.h"

// #include "Count.h"
// #include "Rank.h"
// #include "ReciprocalRank.h"

using namespace std;


namespace meager::main::evaluation::metric::tracker {

    struct Set {
        tracker::TrackerBase** trackers;
        INT length;
        string label;

        Set(tracker::TrackerBase** trackers, INT length, string label) {
            this->trackers = trackers;
            this->length = length;
            this->label = label;
        }

        void update(score::Score* score) {
            for (INT i = 0; i < length; i++) {
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
            printf("%s", prefix.c_str());

            for (INT i = 0; i < length; i++) {
                // printf("\t%f", trackers[i]->divide(nTriples));
                printf("\t%f", *trackers[i] / nTriples);
            }

            printf("\n");
        }
    };

}

#endif
