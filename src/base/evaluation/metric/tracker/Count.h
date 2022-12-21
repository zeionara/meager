#ifndef EVALUATION_METRIC_TRACKER_COUNT_H
#define EVALUATION_METRIC_TRACKER_COUNT_H

#include <iostream>

#include "Tracker.h"

using namespace std;

namespace meager::main::evaluation::metric {

    namespace count {

        string const name = "top_n";

    }

    namespace tracker {

        struct Count: Tracker<INT> {

            INT n;

            Count(INT n): Tracker() {
                this->n = n;
            }

            void update(score::Score* score) {
                if (score->falsePositive < n) {
                    value += 1;
                }
            }

            string getLabel() {
                return count::name + "@" + to_string(n);
            }

            string getName() {
                return count::name;
            }

            Metric getId() {
                return Metric::Count;
            }
        };

    }

}

#endif
