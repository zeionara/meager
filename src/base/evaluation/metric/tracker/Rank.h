#ifndef EVALUATION_METRIC_TRACKER_RANK_H
#define EVALUATION_METRIC_TRACKER_RANK_H

#include "Tracker.h"

namespace meager::main::evaluation::metric {

    namespace rank {

        string const name = "rank";

    }

    namespace tracker {

        struct Rank: Tracker<INT> {

            Rank(): Tracker() {};

            void update(score::Score* score) {
                value += score->falsePositive + 1;
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

}

#endif
