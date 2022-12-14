#include "../evaluation/link-prediction/Evaluator.h"
#include "../evaluation/main.h"

#include "corpus.h"

#define invalidArgument invalid_argument

// using namespace meager::main::utils;

namespace meager::main::api::evaluation {

    main::evaluation::link_prediction::Evaluator<INT>* evaluator;

    void init(main::evaluation::metric::score::SetMaker makeMetricSetTracker, main::evaluation::task::Task task, subset::Type subset, bool verbose) {
        if (subset == subset::Type::train) {
            throw invalidArgument("Cannot use train subset for evaluation");
        }
        if (task != main::evaluation::task::Task::LinkPrediction) {
            throw invalidArgument("Only link prediction task is supported");
        }

        evaluator = new main::evaluation::link_prediction::Evaluator<INT>(
            corpus::corpus,
            corpus::corpus->train,
            subset == subset::Type::test ? corpus::corpus->test : subset == subset::Type::valid ? corpus::corpus->valid : throw invalidArgument("Unsupported subset type, cannot evaluate"),
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

    main::evaluation::metric::tree::Root* computeMetrics(bool verbose) {
        return evaluator->getMetricTree();
    }

}
