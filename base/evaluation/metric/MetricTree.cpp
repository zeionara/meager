#include "MetricTree.h"

MetricTreeRoot::~MetricTreeRoot() {
    delete tree;
}

MetricTreeNode::~MetricTreeNode() {
    delete value;
}
