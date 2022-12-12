#ifndef API_EVALUATOR_H
#define API_EVALUATOR_H

#include "../evaluation/Evaluator.h"
#include "../evaluation/main.h"

using namespace meager::main::utils;

extern
Evaluator<INT>* evaluator;

void initEvaluator(MetricSetTrackerMaker makeMetricSetTracker, EvaluationTask task, SubsetType subset = SubsetType::test, bool verbose = false);

sampling::batch::Triple* trial(triple::Component tripleComponent, bool verbose = false);

void evaluate(triple::Component tripleComponent, REAL* predictions, bool reverse, bool verbose = false);

MetricTreeRoot* computeMetrics(bool verbose);

#endif
