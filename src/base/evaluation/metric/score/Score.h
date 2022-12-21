#ifndef EVALUATION_METRIC_SCORE_SCORE_H
#define EVALUATION_METRIC_SCORE_SCORE_H

#include <string>
#include <optional>
#include <unordered_map>

#include "Container.h"

#include "../Tree.h"

using namespace std;

namespace meager::main::evaluation::metric::score {

    struct Score: Container {

        INT truePositive;
        INT falsePositive;

        INT trueNegative;
        INT falseNegative;

        tracker::Set* metrics;

        Score(tracker::Set* metrics) {
            resetValue();
            this->metrics = metrics;
        }

        void updateMetrics();
        void resetMetrics();

        void resetValue() {
            truePositive = falsePositive = trueNegative = falseNegative = 0;
        }

        void resetScore() {
            resetValue();
        }

        void reset() {
            resetValue();
            resetMetrics();
        };

        tree::Tree* getTree() {
            return new tree::Tree(nullptr, -1, metrics);
        }

    };

}

#endif
