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
