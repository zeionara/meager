#ifndef EVALUATION_METRIC_TRACKER_RECIPROCAL_RANK_H
#define EVALUATION_METRIC_TRACKER_RECIPROCAL_RANK_H

#include "Tracker.h"

namespace meager::main::evaluation::metric {

    namespace reciprocal_rank {

        string const name = "reciprocal_rank";

    }

    namespace tracker {

        struct ReciprocalRank: Tracker<REAL> {

            ReciprocalRank(): Tracker() {};

            void update(score::Score* score) {
                value += 1.0 / (score->falsePositive + 1);
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

}

#endif
