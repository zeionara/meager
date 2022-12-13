#ifndef EVALUATION_METRIC_TREE_H
#define EVALUATION_METRIC_TREE_H

#include <string>
#include <stdexcept>

#include "../../types.h"

#define invalidArgument invalid_argument

using namespace std;

namespace meager::main::evaluation::metric {

    namespace tracker {

        struct Set;

    }

    namespace tree {

        struct Tree;

        struct Root {

            Tree* tree;
            INT normalizationCoefficient;

            Root(Tree* tree, INT normalizationCoefficient) {
                this->tree = tree;
                this->normalizationCoefficient = normalizationCoefficient;
            }

            ~Root();

        };

        struct Node {

            string label;
            Tree* value;

            Node(string label, Tree* value) {
                this->label = label;
                this->value = value;
            }

            ~Node();

        };

        struct Tree {

            Node** nodes;
            INT length;
            tracker::Set* metrics;

            Tree(Node** nodes = nullptr, INT length = -1, tracker::Set* metrics = nullptr) {
                if ((metrics == nullptr && nodes == nullptr) || (metrics != nullptr && nodes != nullptr)) {
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

            ~Tree() {
                if (metrics == nullptr) {
                    delete [] nodes;
                    // metrics may be still needed for other objects (e.g. Evaluator, that's why they are not deleted)
                }
            }

        };

    }

}

#endif
