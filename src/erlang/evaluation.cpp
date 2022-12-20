#include "erl_nif.h"

#include "../base/api/evaluation.h"

#include "utils/main.h"
#include "utils/nif/decode.h"
#include "utils/nif/complete.h"

#include "utils/encode/metric.h"
#include "utils/decode/metric.h"

// #include "sampling.h"
#include "utils/main.h"

namespace meager::erlang::api::evaluation {

    extern ERL_NIF_TERM
    init(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {

            main::evaluation::task::Task task = main::evaluation::task::decodeName(utils::nif::decode::atom(env, argv[1]));
            subset::Type subset = subset::decodeName(utils::nif::decode::atom(env, argv[2]));
            bool verbose = utils::nif::decode::boolean(env, argv[3]);

            main::api::evaluation::init([env, argv](string label){
                auto trackers = utils::decode::metric::metrics(env, argv[0]);
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
            main::sampling::batch::Triple* tripleBatch = main::api::evaluation::trial(
                triple::decodeComponent(utils::nif::decode::atom(env, argv[0])),
                utils::nif::decode::boolean(env, argv[1])
            );

            ERL_NIF_TERM encoded = utils::encode::tripleBatch(env, tripleBatch, false);

            return utils::nif::complete::success(env, encoded);
        } catch (invalidArgument& e) {
            return utils::nif::complete::error(env, e.what());
        }
    }

    extern ERL_NIF_TERM
    evaluate(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            utils::List<REAL>* predictions = utils::nif::decode::list<REAL>(env, argv[1], [](ErlNifEnv* env, ERL_NIF_TERM prediction){
                double decodedPrediction;
                enif_get_double(env, prediction, &decodedPrediction);
                return (REAL)decodedPrediction;
            });

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

    extern ERL_NIF_TERM
    computeMetrics(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            main::evaluation::metric::tree::Root* root = main::api::evaluation::computeMetrics(
                utils::nif::decode::boolean(env, argv[0])
            );

            ERL_NIF_TERM encodedTree = utils::encode::metric::tree(env, root->tree, root->normalizationCoefficient);

            delete root;

            return utils::nif::complete::success(env, encodedTree);
        } catch (invalidArgument& e) {
            return utils::nif::complete::error(env, e.what());
        }

    }

}
