#include "../evaluation/Evaluator.h"
#include "../evaluation/main.h"

#include "corpus.h"

#define invalidArgument invalid_argument

using namespace meager::main::utils;

Evaluator<INT>* evaluator;

void initEvaluator(MetricSetTrackerMaker makeMetricSetTracker, EvaluationTask task, SubsetType subset, bool verbose) {
    if (subset == SubsetType::train) {
        throw invalidArgument("Cannot use train subset for evaluation");
    }
    if (task != LINK_PREDICTION) {
        throw invalidArgument("Only link prediction task is supported");
    }

    evaluator = new Evaluator<INT>(
        corpus,
        corpus->train,
        subset == SubsetType::test ? corpus->test : subset == SubsetType::valid ? corpus->valid : throw invalidArgument("Unsupported subset type, cannot evaluate"),
        makeMetricSetTracker
    );
}

sampling::batch::Triple* trial(triple::Component tripleComponent, bool verbose) {
    switch (tripleComponent) {
        case triple::Component::head:
            return evaluator->head->makeBatch();
        case triple::Component::tail:
            return evaluator->tail->makeBatch();
        default:
            throw invalidArgument("Cannot trial passed triple element");
    }
}

void evaluate(triple::Component tripleComponent, REAL* predictions, bool reverse, bool verbose) {
    switch (tripleComponent) {
        case triple::Component::head:
            evaluator->head->evaluate(predictions, reverse, verbose);
            break;
        case triple::Component::tail:
            evaluator->tail->evaluate(predictions, reverse, verbose);
            break;
        default:
            throw invalidArgument("Cannot evaluate passed triple element");
    }
}

MetricTreeRoot* computeMetrics(bool verbose) {
    return evaluator->getMetricTree();
}
