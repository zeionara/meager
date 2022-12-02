#include "../evaluation/Evaluator.h"

#include "corpus.h"

#define invalidArgument invalid_argument

Evaluator<INT>* evaluator;

void initEvaluator(MetricSetTrackerMaker makeMetricSetTracker, SubsetType subset, bool verbose) {
    if (subset == train) {
        throw invalidArgument("Cannot use train subset for evaluation");
    }

    evaluator = new Evaluator<INT>(
        corpus,
        corpus->train,
        subset == test ? corpus->test : subset == valid ? corpus->valid : throw invalidArgument("Unsupported subset type, cannot evaluate"),
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
