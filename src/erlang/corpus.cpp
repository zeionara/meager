#include "erl_nif.h"

#include "utils/nif/decode.h"
#include "utils/nif/complete.h"

#include "../base/api/corpus.h"

namespace meager::erlang::api::corpus {

    extern ERL_NIF_TERM
    init(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            main::api::corpus::init(
                utils::nif::decode::string(env, argv[0], argv[1]),
                utils::nif::decode::boolean(env, argv[2]),
                utils::nif::decode::boolean(env, argv[3])
            );
        } catch (invalid_argument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    extern ERL_NIF_TERM
    importFilter(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            main::api::corpus::importFilter(
                utils::nif::decode::boolean(env, argv[0]),
                utils::nif::decode::boolean(env, argv[1])
            );
        } catch (invalid_argument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    extern ERL_NIF_TERM
    importPattern(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            main::api::corpus::importPattern(utils::nif::decode::boolean(env, argv[0]));
        } catch (invalid_argument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    extern ERL_NIF_TERM
    importTrain(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            main::api::corpus::importTrain(
                utils::nif::decode::boolean(env, argv[0]),
                utils::nif::decode::boolean(env, argv[1])
            );
        } catch (invalid_argument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    extern ERL_NIF_TERM
    importTriples(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            main::api::corpus::importTriples(
                subset::decodeName(utils::nif::decode::atom(env, argv[0])),
                utils::nif::decode::boolean(env, argv[1])
            );
        } catch (invalid_argument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    extern ERL_NIF_TERM
    importTypes(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            main::api::corpus::importTypes(
                utils::nif::decode::boolean(env, argv[0])
            );
        } catch (invalid_argument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    extern ERL_NIF_TERM
    countEntities(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            return utils::nif::complete::success(
                env,
                enif_make_long(
                    env,
                    main::api::corpus::countEntities(
                        utils::nif::decode::boolean(env, argv[0])
                    )
                )
            );
        } catch (invalid_argument& e) {
            return utils::nif::complete::error(env, e.what());
        }
    }

    extern ERL_NIF_TERM
    countRelations(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            return utils::nif::complete::success(
                env,
                enif_make_long(
                    env,
                    main::api::corpus::countRelations(
                        utils::nif::decode::boolean(env, argv[0])
                    )
                )
            );
        } catch (invalid_argument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    extern ERL_NIF_TERM
    countTriples_1(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            return utils::nif::complete::success(
                env,
                enif_make_long(
                    env,
                    main::api::corpus::countTriples(
                        utils::nif::decode::boolean(env, argv[0])
                    )
                )
            );
        } catch (invalid_argument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    extern ERL_NIF_TERM
    countTriples_2(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            return utils::nif::complete::success(
                env,
                enif_make_long(
                    env,
                    main::api::corpus::countTriples(
                        subset::decodeName(utils::nif::decode::atom(env, argv[0])),
                        utils::nif::decode::boolean(env, argv[1])
                    )
                )
            );
        } catch (invalid_argument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

}
