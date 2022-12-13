#ifndef EVALUATION_METRIC_TRACKER_RECIPROCAL_RANK_H
#define EVALUATION_METRIC_TRACKER_RECIPROCAL_RANK_H

#include "Tracker.h"

namespace meager::main::evaluation::reciprocal_rank {

    string const name = "reciprocal_rank";

}

namespace meager::main::evaluation::metric::tracker {

    struct ReciprocalRank: Tracker<REAL> {

        /// ReciprocalRankMetricTracker(string label): MetricTracker(label) {};
        ReciprocalRank(): Tracker() {};
        // ReciprocalRankMetricTracker(): MetricTracker() {};

        void update(score::Score* score) {
            value += 1.0 / (score->value + 1);
        }

        string getLabel() {
            return reciprocal_rank::name;
        }

        string getName() {
            return reciprocal_rank::name;
        }

        Metric getId() {
            return Metric::ReciprocalRank;
        }
    };

}

#endif
