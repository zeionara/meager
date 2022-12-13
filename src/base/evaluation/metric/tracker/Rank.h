#ifndef EVALUATION_METRIC_TRACKER_RANK_H
#define EVALUATION_METRIC_TRACKER_RANK_H

#include "Tracker.h"

namespace meager::main::evaluation::metric::rank {

    string const name = "rank";

}

namespace meager::main::evaluation::metric::tracker {

    struct Rank: Tracker<INT> {

        Rank(): Tracker() {};
        // RankMetricTracker(string label): MetricTracker(label) {};
        // RankMetricTracker(): MetricTracker() {};

        void update(score::Score* score) {
            value += score->value + 1;
        }

        string getLabel() {
            return rank::name;
        }

        string getName() {
            return rank::name;
        }

        Metric getId() {
            return Metric::Rank;
        }
    };

}

#endif
