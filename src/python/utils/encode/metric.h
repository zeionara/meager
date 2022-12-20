#ifndef UTILS_ENCODE_METRIC_H
#define UTILS_ENCODE_METRIC_H

#include "../../../base/evaluation/metric/tracker/Count.h"
#include "../../../base/evaluation/metric/tracker/Set.h"

#include "../main.h"

#include <cstring>

using namespace std;

namespace meager::python::utils::encode::metric {

    template <typename T>
    void* metric(std::string name, T* parameters = NULL, unsigned char length = 0) {
        const unsigned short nameSize = name.length() + 1;
        const unsigned short paramsSize = sizeof(T) * length;

        unsigned char* result = new unsigned char[1 + nameSize + paramsSize];

        // set up number of params

        result[0] = length;

        // set up metric name

        char* copiedName = (char*)(result + 1);
        strcpy(copiedName, name.c_str());

        // set up param values

        T* params = (T*)(result + 1 + nameSize);

        for (unsigned char i = 0; i < length; i++) {
            params[i] = parameters[i];
        }

        return result;
    }

    template <typename T>
    void* metric(meager::main::evaluation::metric::tracker::TrackerBase* metric) {
        switch (metric->getId()) {
            case meager::main::evaluation::metric::Metric::Count: {
                T parameters[1] = {(T)(((meager::main::evaluation::metric::tracker::Count*)metric)->n)};
                return metric::metric(metric->getName(), parameters, 1);
                break;
            }
            default:
                return metric::metric<T>(metric->getName());
        }
    }

    struct EncodedNode;

    struct EncodedTree {
        unsigned char* content;
        EncodedNode** values;
        unsigned long length;

        EncodedTree(unsigned char* content, EncodedNode** values = NULL) {
            this->content = content;
            this->values = values;
            this->length = content[0] & 0x7f;
        }

        ~EncodedTree();
    };

    struct EncodedNode {
        void* treeContent;
        char* label;

        EncodedTree* tree;

        EncodedNode(EncodedTree* tree, char* label) {
            this->treeContent = tree->content;
            this->tree = tree;
            this->label = label;
        }

        ~EncodedNode() {
            delete tree;
            delete [] label;
        }
    };

    template <typename M, typename T>
    EncodedTree* tree(meager::main::evaluation::metric::tree::Tree* tree, INT normalizationCoefficient) {
        if (tree->length > 127) {
            throw invalidArgument("Cannot encode node with more than 127 descendants");
        }

        if (tree->nodes != NULL) {
            const unsigned long length = tree->length;
            auto nodes = tree->nodes;

            unsigned char* result = new unsigned char[sizeof(void*) * length + 1];

            result[0] = length;

            void** encodedNodes = (void**)(result + 1);

            for (INT i = 0; i < length; i++) {
                auto node = nodes[i];

                std::string label = node->label;

                EncodedNode* encodedNode = new EncodedNode(
                    encode::metric::tree<M, T>(node->value, normalizationCoefficient),
                    meager::python::utils::string::toDynamicCharArray(label)
                );

                encodedNodes[i] = encodedNode;
            }

            return new EncodedTree(result, (EncodedNode**)encodedNodes);
        } else {

            auto metrics = tree->metrics;

            if (metrics != NULL) {
                const unsigned long length = metrics->length;

                const unsigned long valuesSize = sizeof(M*) * length;
                const unsigned long namesSize = sizeof(char*) * length;

                unsigned char* result = new unsigned char[1 + valuesSize + namesSize];

                result[0] = length;
                result[0] |= (unsigned char)0x80;

                M* values = (M*)(result + 1);
                char** names = (char**)(result + 1 + valuesSize);

                for (INT i = 0; i < tree->metrics->length; i++) {
                    auto metric = tree->metrics->trackers[i];

                    values[i] = metric->divide(normalizationCoefficient);
                    names[i] = (char*)meager::python::utils::encode::metric::metric<T>(metric); // (char*)encodeMetric(metric).c_str();
                }

                return new EncodedTree(result);
            }

            throw invalidArgument("If list of nodes is not set then metrics must be set");

        }
        throw invalidArgument("Each metric tree node must either contain links to other nodes either contain a list of metrics");
    }

}

#endif
