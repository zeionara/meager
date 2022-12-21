#include "erl_nif.h"

#include "../../../base/evaluation/metric/tracker/Count.h"
#include "../../../base/evaluation/metric/tracker/Set.h"

namespace meager::erlang::utils::encode::metric {

    ERL_NIF_TERM metric(ErlNifEnv* env, main::evaluation::metric::tracker::TrackerBase* metric) {
        switch (metric->getId()) {
            case main::evaluation::metric::Metric::Count:
                return enif_make_tuple2(env, enif_make_atom(env, metric->getName().c_str()), enif_make_long(env, ((meager::main::evaluation::metric::tracker::Count*)metric)->n));
            default:
                return enif_make_atom(env, metric->getName().c_str());
        }
    }

    ERL_NIF_TERM tree(ErlNifEnv* env, main::evaluation::metric::tree::Tree* tree, INT normalizationCoefficient) {
        if (tree->nodes != nullptr) {
            ERL_NIF_TERM* encodedChildren = new ERL_NIF_TERM[tree->length]();

            for (INT i = 0; i < tree->length; i++) {
                string label = tree->nodes[i]->label;

                main::evaluation::metric::tree::Tree* subtree = tree->nodes[i]->value;

                ERL_NIF_TERM encodedChild = enif_make_tuple2(env, enif_make_atom(env, label.c_str()), metric::tree(env, subtree, normalizationCoefficient));
                encodedChildren[i] = encodedChild;
            }

            return enif_make_list_from_array(env, encodedChildren, tree->length);
        } else {
            ERL_NIF_TERM* encodedMetrics = new ERL_NIF_TERM[tree->metrics->length]();

            if (tree->metrics) {
                for (INT i = 0; i < tree->metrics->length; i++) {
                    main::evaluation::metric::tracker::TrackerBase* metric = tree->metrics->trackers[i];

                    // evaluation::metric::Metric id = metric->getId();

                    encodedMetrics[i] = enif_make_tuple2(env, metric::metric(env, metric), enif_make_double(env, *metric / normalizationCoefficient));
                }
            }

            return enif_make_list_from_array(env, encodedMetrics, tree->metrics->length);
        }
        throw invalidArgument("Each metric tree node must either contain links to other nodes either contain a list of metrics");
    }

}
