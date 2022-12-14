#ifndef API_EVALUATOR_H
#define API_EVALUATOR_H

#include "../evaluation/link-prediction/Evaluator.h"
#include "../evaluation/main.h"

using namespace meager::main::utils;

extern
evaluation::link_prediction::Evaluator<INT>* evaluator;

void initEvaluator(evaluation::metric::score::SetMaker makeMetricSetTracker, evaluation::task::Task task, subset::Type subset = subset::Type::test, bool verbose = false);

sampling::batch::Triple* trial(triple::Component tripleComponent, bool verbose = false);

void evaluate(triple::Component tripleComponent, REAL* predictions, bool reverse, bool verbose = false);

evaluation::metric::tree::Root* computeMetrics(bool verbose);

#endif
