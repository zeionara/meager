#ifndef API_EVALUATOR_H
#define API_EVALUATOR_H

#include "../evaluation/Evaluator.h"

extern
Evaluator<INT>* evaluator;

void initEvaluator(MetricSetTrackerMaker makeMetricSetTracker, SubsetType subset = test, bool verbose = false);

TripleBatch* trial(TripleElement element, bool verbose = false);

void evaluate(TripleElement element, REAL* predictions, bool reverse, bool verbose = false);

MetricTree* computeMetrics(bool verbose);

#endif
