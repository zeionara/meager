#ifndef EVALUATION_METRIC_SCORE_GROUP
#define EVALUATION_METRIC_SCORE_GROUP

#include <functional>
#include <unordered_map>

// #include "Score.h"
// #include "Container.h"

#include "../tracker/Set.h"

namespace meager::main::evaluation::metric::score {

    typedef function<tracker::Set*(string)> SetMaker;

    string const UNFILTERED = "unfiltered";
    string const FILTERED = "filtered";
    string const SEPARATOR = "_";

    struct Group: Container {

        Score* filtered;
        Score* unfiltered;

        string label;

        Group(SetMaker makeMetricSetTracker, string label) {
            unfiltered = new Score(makeMetricSetTracker(label + SEPARATOR + UNFILTERED));
            filtered = new Score(makeMetricSetTracker(label + SEPARATOR + FILTERED));
            this->label = label;
        }

        void updateMetrics() {
            filtered->updateMetrics();
            unfiltered->updateMetrics();
        }

        void resetScore() {
            filtered->resetValue();
            unfiltered->resetValue();
        }

        void resetMetrics() {
            filtered->resetMetrics();
            unfiltered->resetMetrics();
        }

        void reset() {
            filtered->reset();
            unfiltered->reset();
        }

        void printMetrics(INT nTriples) {
            unfiltered->metrics->printMetrics(label + SEPARATOR + UNFILTERED, nTriples);
            filtered->metrics->printMetrics(label + SEPARATOR + FILTERED, nTriples);
        }

        tree::Tree* getTree() {
            INT const length = 2;
            INT i = 0;

            tree::Node** nodes = new tree::Node*[length];

            nodes[i++] = new tree::Node(UNFILTERED, unfiltered->getTree());
            nodes[i++] = new tree::Node(FILTERED, filtered->getTree());

            return new tree::Tree(nodes, length);
        }

    };

}

#endif
