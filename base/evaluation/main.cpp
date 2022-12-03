#include <stdexcept>

#include "main.h"

#define invalidArgument invalid_argument

EvaluationTask decodeEvaluationTask(string name) {

    if (name == "link_prediction") {
        return LINK_PREDICTION;
    }

    if (name == "triple_classification") {
        return TRIPLE_CLASSIFICATION;
    }

    throw invalidArgument("Unknown task name provided: " + name);
}
