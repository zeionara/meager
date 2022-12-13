#include <stdexcept>

#include "main.h"

#include "triple-classification/main.h"
#include "link-prediction/main.h"

#define invalidArgument invalid_argument

namespace meager::main::evaluation::task {

    Task decodeName(string name) {

        if (name == link_prediction::name) {
            return Task::LinkPrediction;
        }

        if (name == triple_classification::name) {
            return Task::TripleClassification;
        }

        throw invalidArgument("Unknown task name provided: " + name);
    }

}
