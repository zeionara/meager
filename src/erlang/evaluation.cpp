#include "erl_nif.h"
#include "utils.h"

// #include "../base/utils/main.h"
#include "../base/api/evaluation.h"

#include "../base/evaluation/metric/main.h"
// #include "../base/evaluation/metric/Metric.h"

#include "../base/evaluation/metric/tracker/Count.h"
#include "../base/evaluation/metric/tracker/Rank.h"
#include "../base/evaluation/metric/tracker/ReciprocalRank.h"

#include "sampling.h"

// using namespace meager;

namespace meager::erlang::api::evaluation {

    extern ERL_NIF_TERM
    init(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {

            main::evaluation::task::Task task = main::evaluation::task::decodeName(utils::nif::decode::atom(env, argv[1]));
            subset::Type subset = subset::decodeName(utils::nif::decode::atom(env, argv[2]));
            bool verbose = utils::nif::decode::boolean(env, argv[3]);

            main::api::evaluation::init([env, argv](string label){
                utils::nif::decode::List<main::evaluation::metric::tracker::TrackerBase*>* trackers = utils::nif::decode::list<main::evaluation::metric::tracker::TrackerBase*>(
                    env, argv[0], [](ErlNifEnv* env, ERL_NIF_TERM metric
                ) -> main::evaluation::metric::tracker::TrackerBase* {
                    int length;
                    const ERL_NIF_TERM* metricDescription;

                    enif_get_tuple(env, metric, &length, &metricDescription);

                    if (length < 1) {
                        throw invalidArgument("Empty metric descriptions are not allowed");
                    }

                    char* metricName = utils::nif::decode::atom(env, metricDescription[0]);

                    switch (main::evaluation::metric::decodeName(metricName)) {
                        case main::evaluation::metric::Metric::Count:
                            if (length > 1)
                                return new main::evaluation::metric::tracker::Count(utils::nif::decode::longInteger(env, metricDescription[1])); 
                            throw invalidArgument("Metric count requires one parameter (top-n)");
                        case main::evaluation::metric::Metric::Rank:
                            if (length < 2)
                                return new main::evaluation::metric::tracker::Rank(); 
                            throw invalidArgument("Metric rank doesn't accept parameters");
                        case main::evaluation::metric::Metric::ReciprocalRank:
                            if (length == 1)
                                return new main::evaluation::metric::tracker::ReciprocalRank(); 
                            throw invalidArgument("Metric reciprocal-rank doesn't accept parameters");
                        default:
                            throw invalidArgument("Unspecified metric tracker for metric " + string(metricName));
                    }
                });

                return new main::evaluation::metric::tracker::Set(trackers->items, trackers->length, label);
            }, task, subset, verbose);

        } catch (invalidArgument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    extern ERL_NIF_TERM
    trial(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            // cout << "foo" << endl;
            main::sampling::batch::Triple* tripleBatch = main::api::evaluation::trial(
                triple::decodeComponent(utils::nif::decode::atom(env, argv[0])),
                utils::nif::decode::boolean(env, argv[1])
            );

            ERL_NIF_TERM encoded = sampling::encodeTripleBatch(env, tripleBatch, false);

            return utils::nif::complete::success(env, encoded);
        } catch (invalidArgument& e) {
            return utils::nif::complete::error(env, e.what());
        }
    }

    extern ERL_NIF_TERM
    evaluate(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            // cout << "Encoding predictions" << endl;
            utils::nif::decode::List<REAL>* predictions = utils::nif::decode::list<REAL>(env, argv[1], [](ErlNifEnv* env, ERL_NIF_TERM prediction){
                double decodedPrediction;
                enif_get_double(env, prediction, &decodedPrediction);
                return (REAL)decodedPrediction;
            });
            // cout << "Encoded predictions" << endl;

            main::api::evaluation::evaluate(
                triple::decodeComponent(utils::nif::decode::atom(env, argv[0])),
                predictions->items,
                utils::nif::decode::boolean(env, argv[2]),
                utils::nif::decode::boolean(env, argv[3])
            );
        } catch (invalidArgument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    ERL_NIF_TERM encodeMetric(ErlNifEnv* env, main::evaluation::metric::tracker::TrackerBase* metric) {
        switch (metric->getId()) {
            case main::evaluation::metric::Metric::Count:
                return enif_make_tuple2(env, enif_make_atom(env, metric->getName().c_str()), enif_make_long(env, ((main::evaluation::metric::tracker::Count*)metric)->n));
            default:
                return enif_make_atom(env, metric->getName().c_str());
        }
    }

    ERL_NIF_TERM encodeMetricTree(ErlNifEnv* env, main::evaluation::metric::tree::Tree* tree, INT normalizationCoefficient) {
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
                main::evaluation::metric::tree::Tree* subtree = tree->nodes[i]->value;

                ERL_NIF_TERM encodedChild = enif_make_tuple2(env, enif_make_atom(env, label.c_str()), encodeMetricTree(env, subtree, normalizationCoefficient));
                encodedChildren[i] = encodedChild;
            }

            return enif_make_list_from_array(env, encodedChildren, tree->length);
        } else {
            ERL_NIF_TERM* encodedMetrics = new ERL_NIF_TERM[tree->metrics->length]();

            if (tree->metrics) {
                for (INT i = 0; i < tree->metrics->length; i++) {
                    main::evaluation::metric::tracker::TrackerBase* metric = tree->metrics->trackers[i];

                    // evaluation::metric::Metric id = metric->getId();

                    encodedMetrics[i] = enif_make_tuple2(env, encodeMetric(env, metric), enif_make_double(env, metric->divide(normalizationCoefficient)));
                }
            }


            return enif_make_list_from_array(env, encodedMetrics, tree->metrics->length);
        }
        throw invalidArgument("Each metric tree node must either contain links to other nodes either contain a list of metrics");
    }

    extern ERL_NIF_TERM
    computeMetrics(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            main::evaluation::metric::tree::Root* root = main::api::evaluation::computeMetrics(
                utils::nif::decode::boolean(env, argv[0])
            );
            // cout << "Started encoding tree" << endl;
            ERL_NIF_TERM encodedTree = encodeMetricTree(env, root->tree, root->normalizationCoefficient);
            delete root;
            // cout << "Finished encoding tree" << endl;
            return utils::nif::complete::success(env, encodedTree);
        } catch (invalidArgument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        // return completed_with_success(env, enif_make_long(env, 17));
    }

}
