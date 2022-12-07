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

TripleBatch* trial(TripleElement element, bool verbose) {
    switch (element) {
        case head:
            return evaluator->head->makeBatch();
        case tail:
            return evaluator->tail->makeBatch();
        default:
            throw invalidArgument("Cannot trial passed triple element");
    }
}

void evaluate(TripleElement element, REAL* predictions, bool reverse, bool verbose) {
    switch (element) {
        case head:
            evaluator->head->evaluate(predictions, reverse, verbose);
            break;
        case tail:
            evaluator->tail->evaluate(predictions, reverse, verbose);
            break;
        default:
            throw invalidArgument("Cannot evaluate passed triple element");
    }
}

MetricTreeRoot* computeMetrics(bool verbose) {
    return evaluator->getMetricTree();
}
