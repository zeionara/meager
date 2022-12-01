#ifndef API_EVALUATOR_H
#define API_EVALUATOR_H

#include "../evaluation/Evaluator.h"

extern
Evaluator<INT>* evaluator;

void initEvaluator(MetricSetTrackerMaker makeMetricSetTracker, SubsetType subset = test, bool verbose = false);

#endif
