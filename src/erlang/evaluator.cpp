#include "erl_nif.h"
#include "utils.h"

#include "../base/utils/main.h"
#include "../base/api/evaluation.h"
#include "../base/evaluation/metric/main.h"
#include "../base/evaluation/metric/Metric.h"

#include "../base/evaluation/metric/tracker/Count.h"
#include "../base/evaluation/metric/tracker/Rank.h"
#include "../base/evaluation/metric/tracker/ReciprocalRank.h"

#include "sampler.h"

using namespace meager;

extern ERL_NIF_TERM
initEvaluator_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {

        evaluation::task::Task task = evaluation::task::decodeName(enif_get_atom_(env, argv[1]));
        subset::Type subset = subset::decodeName(enif_get_atom_(env, argv[2]));
        bool verbose = enif_get_bool(env, argv[3]);

        main::api::evaluation::init([env, argv](string label){
            List<evaluation::metric::tracker::TrackerBase*>* trackers = decodeList<evaluation::metric::tracker::TrackerBase*>(
                env, argv[0], [](ErlNifEnv* env, ERL_NIF_TERM metric
            ) -> evaluation::metric::tracker::TrackerBase* {
                int length;
                const ERL_NIF_TERM* metricDescription;

                enif_get_tuple(env, metric, &length, &metricDescription);

                if (length < 1) {
                    throw invalidArgument("Empty metric descriptions are not allowed");
                }

                char* metricName = enif_get_atom_(env, metricDescription[0]);

                switch (evaluation::metric::decodeName(metricName)) {
                    case evaluation::metric::Metric::Count:
                        if (length > 1)
                            return new evaluation::metric::tracker::Count(enif_get_long_(env, metricDescription[1])); 
                        throw invalidArgument("Metric count requires one parameter (top-n)");
                    case evaluation::metric::Metric::Rank:
                        if (length < 2)
                            return new evaluation::metric::tracker::Rank(); 
                        throw invalidArgument("Metric rank doesn't accept parameters");
                    case evaluation::metric::Metric::ReciprocalRank:
                        if (length == 1)
                            return new evaluation::metric::tracker::ReciprocalRank(); 
                        throw invalidArgument("Metric reciprocal-rank doesn't accept parameters");
                    default:
                        throw invalidArgument("Unspecified metric tracker for metric " + string(metricName));
                }
            });

            return new evaluation::metric::tracker::Set(trackers->items, trackers->length, label);
        }, task, subset, verbose);

    } catch (invalidArgument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
trial_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        // cout << "foo" << endl;
        sampling::batch::Triple* tripleBatch = main::api::evaluation::trial(
            triple::decodeComponent(enif_get_atom_(env, argv[0])),
            enif_get_bool(env, argv[1])
        );

        ERL_NIF_TERM encoded = encodeTripleBatch(env, tripleBatch, false);

        return completed_with_success(env, encoded);
    } catch (invalidArgument& e) {
        return completed_with_error(env, e.what());
    }
}

extern ERL_NIF_TERM
evaluate_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        // cout << "Encoding predictions" << endl;
        List<REAL>* predictions = decodeList<REAL>(env, argv[1], [](ErlNifEnv* env, ERL_NIF_TERM prediction){
            double decodedPrediction;
            enif_get_double(env, prediction, &decodedPrediction);
            return (REAL)decodedPrediction;
        });
        // cout << "Encoded predictions" << endl;

        main::api::evaluation::evaluate(
            triple::decodeComponent(enif_get_atom_(env, argv[0])),
            predictions->items,
            enif_get_bool(env, argv[2]),
            enif_get_bool(env, argv[3])
        );
    } catch (invalidArgument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

ERL_NIF_TERM encodeMetric(ErlNifEnv* env, evaluation::metric::tracker::TrackerBase* metric) {
    switch (metric->getId()) {
        case evaluation::metric::Metric::Count:
            return enif_make_tuple2(env, enif_make_atom(env, metric->getName().c_str()), enif_make_long(env, ((evaluation::metric::tracker::Count*)metric)->n));
        default:
            return enif_make_atom(env, metric->getName().c_str());
    }
}

ERL_NIF_TERM encodeMetricTree(ErlNifEnv* env, evaluation::metric::tree::Tree* tree, INT normalizationCoefficient) {
    // cout << "--" << endl;
    // cout << tree->nodes << endl;
    // cout << tree->length << endl;
    // cout << tree->metrics << endl;
    // cout << "++" << endl;
    if (tree->nodes != nullptr) {
        // cout << tree->length << endl;
        ERL_NIF_TERM* encodedChildren = new ERL_NIF_TERM[tree->length]();

        for (INT i = 0; i < tree->length; i++) {
            string label = tree->nodes[i]->label;
            // cout << "label = " << label << endl;
            evaluation::metric::tree::Tree* subtree = tree->nodes[i]->value;

            ERL_NIF_TERM encodedChild = enif_make_tuple2(env, enif_make_atom(env, label.c_str()), encodeMetricTree(env, subtree, normalizationCoefficient));
            encodedChildren[i] = encodedChild;
        }

        return enif_make_list_from_array(env, encodedChildren, tree->length);
    } else {
        ERL_NIF_TERM* encodedMetrics = new ERL_NIF_TERM[tree->metrics->length]();

        if (tree->metrics) {
            for (INT i = 0; i < tree->metrics->length; i++) {
                evaluation::metric::tracker::TrackerBase* metric = tree->metrics->trackers[i];

                // evaluation::metric::Metric id = metric->getId();

                encodedMetrics[i] = enif_make_tuple2(env, encodeMetric(env, metric), enif_make_double(env, metric->divide(normalizationCoefficient)));
            }
        }


        return enif_make_list_from_array(env, encodedMetrics, tree->metrics->length);
    }
    throw invalidArgument("Each metric tree node must either contain links to other nodes either contain a list of metrics");
}

extern ERL_NIF_TERM
computeMetrics_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        evaluation::metric::tree::Root* root = main::api::evaluation::computeMetrics(
            enif_get_bool(env, argv[0])
        );
        // cout << "Started encoding tree" << endl;
        ERL_NIF_TERM encodedTree = encodeMetricTree(env, root->tree, root->normalizationCoefficient);
        delete root;
        // cout << "Finished encoding tree" << endl;
        return completed_with_success(env, encodedTree);
    } catch (invalidArgument& e) {
        return completed_with_error(env, e.what());
    }

    // return completed_with_success(env, enif_make_long(env, 17));
}
