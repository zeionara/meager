#ifndef EVALUATION_METRIC_SCORE_SCORE_H
#define EVALUATION_METRIC_SCORE_SCORE_H

#include <string>
#include <optional>
#include <unordered_map>

// #include "../../../types.h"
#include "Container.h"

#include "../Tree.h"

using namespace std;

namespace meager::main::evaluation::metric::score {

    struct Score: Container {

        INT value;
        tracker::Set* metrics;

        Score(tracker::Set* metrics) {
            value = 0;
            this->metrics = metrics;
        }

        void updateMetrics();

        void updateMetrica() {
            
        }

        void resetValue() {
            value = 0;
        }

        void resetScore() {
            resetValue();
        }

        void resetMetrics();

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
