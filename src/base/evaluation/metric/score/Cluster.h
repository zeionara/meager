#ifndef EVALUATION_METRIC_SCORE_CLUSTER_H
#define EVALUATION_METRIC_SCORE_CLUSTER_H

#include "Group.h"
// #include "Container.h"

namespace meager::main::evaluation::metric::score {

    string const CONSTRAINED = "constrained";
    string const UNCONSTRAINED = "unconstrained";

    struct Cluster: Container {

        Group* constrained;
        Group* unconstrained;

        string label;

        Cluster(SetMaker makeMetricSetTracker, string label) {
            unconstrained = new Group(makeMetricSetTracker, label + SEPARATOR + UNCONSTRAINED);
            constrained = new Group(makeMetricSetTracker, label + SEPARATOR + CONSTRAINED);
            this->label = label;
        }

        void updateMetrics() {
            // cout << "Updating metrics in cluster" << endl;
            constrained->updateMetrics();
            // cout << "Updating metrics in cluster" << endl;
            unconstrained->updateMetrics();
        }

        void resetScore() {
            constrained->resetScore();
            unconstrained->resetScore();
        }

        void resetMetrics() {
            constrained->resetMetrics();
            unconstrained->resetMetrics();
        }

        void reset() {
            constrained->reset();
            unconstrained->reset();
        }

        void printMetrics(INT nTriples) {
            unconstrained->printMetrics(nTriples);
            constrained->printMetrics(nTriples);
        }

        tree::Tree* getTree() {
            INT const length = 2;
            INT i = 0;

            tree::Node** nodes = new tree::Node*[length];

            nodes[i++] = new tree::Node(UNCONSTRAINED, unconstrained->getTree());
            nodes[i++] = new tree::Node(CONSTRAINED, constrained->getTree());

            return new tree::Tree(nodes, length);
        }

    };

}

#endif
