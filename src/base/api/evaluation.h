#ifndef API_EVALUATION_H
#define API_EVALUATION_H

#include "../evaluation/link-prediction/Evaluator.h"
#include "../evaluation/main.h"

// using namespace meager::main::utils;

namespace meager::main::api::evaluation {

    extern
    main::evaluation::link_prediction::Evaluator<INT>* evaluator;

    void init(main::evaluation::metric::score::SetMaker makeMetricSetTracker, main::evaluation::task::Task task, subset::Type subset = subset::Type::test, bool verbose = false);

    sampling::batch::Triple* trial(triple::Component tripleComponent, bool verbose = false);

    void evaluate(triple::Component tripleComponent, REAL* predictions, bool reverse, bool verbose = false);

    main::evaluation::metric::tree::Root* computeMetrics(bool verbose);

}

#endif
