#ifndef API_EVALUATOR_H
#define API_EVALUATOR_H

#include "../evaluation/Evaluator.h"
#include "../evaluation/main.h"

extern
Evaluator<INT>* evaluator;

void initEvaluator(MetricSetTrackerMaker makeMetricSetTracker, EvaluationTask task, SubsetType subset = test, bool verbose = false);

TripleBatch* trial(TripleElement element, bool verbose = false);

void evaluate(TripleElement element, REAL* predictions, bool reverse, bool verbose = false);

MetricTreeRoot* computeMetrics(bool verbose);

#endif
