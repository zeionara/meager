#ifndef EVALUATION_METRIC_METRIC_TREE_H
#define EVALUATION_METRIC_METRIC_TREE_H

#include <string>
#include <stdexcept>

#include "../../types.h"

#define invalidArgument invalid_argument

using namespace std;

struct MetricSetTracker;
struct MetricTree;

struct MetricTreeRoot {

    MetricTree* tree;
    INT normalizationCoefficient;

    MetricTreeRoot(MetricTree* tree, INT normalizationCoefficient) {
        this->tree = tree;
        this->normalizationCoefficient = normalizationCoefficient;
    }

    ~MetricTreeRoot();

};

struct MetricTreeNode {

    string label;
    MetricTree* value;

    MetricTreeNode(string label, MetricTree* value) {
        this->label = label;
        this->value = value;
    }

    ~MetricTreeNode();

};

struct MetricTree {

    MetricTreeNode** nodes;
    INT length;
    MetricSetTracker* metrics;

    MetricTree(MetricTreeNode** nodes = nullptr, INT length = -1, MetricSetTracker* metrics = nullptr) {
        if (metrics == nullptr && nodes == nullptr || metrics != nullptr and nodes != nullptr) {
            throw invalidArgument("Either nodes list either metrics list must be specified");
        }
        if (nodes != nullptr && length < 0) {
            throw invalidArgument("If list of child nodes is specified, then length must be a non-negative number");
        }
        if (nodes == nullptr && length >= 0) {
            throw invalidArgument("If list of child nodes is not specified, then length must be a negative number");
        }

        this->nodes = nodes;
        this->length = length;
        this->metrics = metrics;
    }

    ~MetricTree() {
        if (metrics == nullptr) {
            delete [] nodes;
            // metrics may be still needed for other objects (e.g. Evaluator, that's why they are not deleted)
        }
    }

};

#endif
