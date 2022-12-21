#include "erl_nif.h"

#include "corpus.h"
#include "sampling.h"
#include "evaluation.h"

namespace meager::erlang::api {

    static ErlNifFunc functions[] = {
        // Corpus

        {"_init_corpus", 4, corpus::init},

        {"_import_filter", 2, corpus::importFilter},
        {"_import_pattern", 1, corpus::importPattern},
        {"_import_types", 1, corpus::importTypes},

        {"_import_train", 2, corpus::importTrain},
        {"_import_triples", 2, corpus::importTriples},

        {"_count_entities", 1, corpus::countEntities},
        {"_count_relations", 1, corpus::countRelations},
        {"_count_triples", 1, corpus::countTriples_1},
        {"_count_triples", 2, corpus::countTriples_2},

        // Sampler

        {"_init_sampler", 6, sampling::init},
        {"_sample", 5, sampling::sample},

        // Evaluator

        {"_init_evaluator", 4, evaluation::init},
        {"_trial", 2, evaluation::trial},
        {"_evaluate", 4, evaluation::evaluate},
        {"_compute_metrics", 1, evaluation::computeMetrics},

    };

}

ERL_NIF_INIT(Elixir.Grapex.Meager, meager::erlang::api::functions, NULL, NULL, NULL, NULL)
