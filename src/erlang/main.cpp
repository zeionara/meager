#include "corpus.h"
#include "sampling.h"
#include "evaluation.h"

namespace meager::erlang::api {

    static ErlNifFunc functions[] = {
        // Corpus

        {"init_corpus", 4, corpus::init},

        {"import_filter", 2, corpus::importFilter},
        {"import_pattern", 1, corpus::importPattern},
        {"import_types", 1, corpus::importTypes},

        {"import_train", 2, corpus::importTrain},
        {"import_triples", 2, corpus::importTriples},

        {"count_entities", 1, corpus::countEntities},
        {"count_relations", 1, corpus::countRelations},
        {"count_triples", 1, corpus::countTriples_1},
        {"count_triples", 2, corpus::countTriples_2},

        // Sampler

        {"init_sampler", 6, sampling::init},
        {"sample", 5, sampling::sample},

        // Evaluator

        {"init_evaluator", 4, evaluation::init},
        {"trial", 2, evaluation::trial},
        {"evaluate", 4, evaluation::evaluate},
        {"compute_metrics", 1, evaluation::computeMetrics},

    };

}

ERL_NIF_INIT(Elixir.Grapex.Meager.Placeholder, meager::erlang::api::functions, NULL, NULL, NULL, NULL)
