#include "erl_nif.h"
#include "utils.h"

#include "../base/utils/main.h"
#include "../base/api/evaluator.h"
#include "../base/evaluation/metric/Metric.h"

#include "sampler.h"

extern ERL_NIF_TERM
initEvaluator_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {

        SubsetType subset = decodeSubsetType(enif_get_atom_(env, argv[1]));
        bool verbose = enif_get_bool(env, argv[2]);

        initEvaluator([env, argv](string label){
            List<MetricTrackerBase*>* trackers = decodeList<MetricTrackerBase*>(env, argv[0], [](ErlNifEnv* env, ERL_NIF_TERM metric) -> MetricTrackerBase* {
                int length;
                const ERL_NIF_TERM* metricDescription;

                enif_get_tuple(env, metric, &length, &metricDescription);

                if (length < 1) {
                    throw invalidArgument("Empty metric descriptions are not allowed");
                }

                char* metricName = enif_get_atom_(env, metricDescription[0]);

                switch (decodeMetric(metricName)) {
                    case Count:
                        if (length > 1)
                            return new CountMetricTracker(enif_get_long_(env, metricDescription[1])); 
                        throw invalidArgument("Metric count requires one parameter (top-n)");
                    case Rank:
                        if (length < 2)
                            return new RankMetricTracker(); 
                        throw invalidArgument("Metric rank doesn't accept parameters");
                    case ReciprocalRank:
                        if (length == 1)
                            return new ReciprocalRankMetricTracker(); 
                        throw invalidArgument("Metric reciprocal-rank doesn't accept parameters");
                    default:
                        throw invalidArgument("Unspecified metric tracker for metric " + string(metricName));
                }
            });

            return new MetricSetTracker(trackers->items, trackers->length, label);
        }, subset, verbose);

    } catch (invalidArgument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
trial_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        // cout << "foo" << endl;
        TripleBatch* tripleBatch = trial(
            decodeTripleElement(enif_get_atom_(env, argv[0])),
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
        List<REAL>* predictions = decodeList<REAL>(env, argv[1], [](ErlNifEnv* env, ERL_NIF_TERM prediction){
            double decodedPrediction;
            enif_get_double(env, prediction, &decodedPrediction);
            return (REAL)decodedPrediction;
        });

        evaluate(
            decodeTripleElement(enif_get_atom_(env, argv[0])),
            predictions->items,
            enif_get_bool(env, argv[2]),
            enif_get_bool(env, argv[3])
        );
    } catch (invalidArgument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

ERL_NIF_TERM encodeMetric(ErlNifEnv* env, MetricTrackerBase* metric) {
    switch (metric->getId()) {
        case Count:
            return enif_make_tuple2(env, enif_make_atom(env, metric->getName().c_str()), enif_make_long(env, ((CountMetricTracker*)metric)->n));
        default:
            return enif_make_atom(env, metric->getName().c_str());
    }
}

ERL_NIF_TERM encodeMetricTree(ErlNifEnv* env, MetricTree* tree, INT normalizationCoefficient) {
    if (tree->children) {
        ERL_NIF_TERM* encodedChildren = new ERL_NIF_TERM[tree->children.value().size()]();
        INT i = 0;

        for (auto child: tree->children.value()) {
            string label = child.first;
            MetricTree* subtree = child.second;

            ERL_NIF_TERM encodedChild = enif_make_tuple2(env, enif_make_atom(env, label.c_str()), encodeMetricTree(env, subtree, normalizationCoefficient));
            encodedChildren[i++] = encodedChild;
        }

        return enif_make_list_from_array(env, encodedChildren, i);
    } else {
        ERL_NIF_TERM* encodedMetrics = new ERL_NIF_TERM[tree->metrics.value()->length]();

        if (tree->metrics) {
            for (INT i = 0; i < tree->metrics.value()->length; i++) {
                MetricTrackerBase* metric = tree->metrics.value()->trackers[i];

                Metric id = metric->getId();

                encodedMetrics[i] = enif_make_tuple2(env, encodeMetric(env, metric), enif_make_double(env, metric->divide(normalizationCoefficient)));
            }
        }


        return enif_make_list_from_array(env, encodedMetrics, tree->metrics.value()->length);
    }
    throw invalidArgument("Each metric tree node must either contain links to other nodes either contain a list of metrics");
}

extern ERL_NIF_TERM
computeMetrics_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        MetricTreeRoot* root = computeMetrics(
            enif_get_bool(env, argv[0])
        );
        return completed_with_success(env, encodeMetricTree(env, root->tree, root->normalizationCoefficient));
    } catch (invalidArgument& e) {
        return completed_with_error(env, e.what());
    }

    // return completed_with_success(env, enif_make_long(env, 17));
}
