#include "erl_nif.h"
#include "utils.h"

#include "../base/api/corpus.h"

using namespace meager;

extern ERL_NIF_TERM
initCorpus_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        main::api::corpus::init(
            enif_get_string_(env, argv[0], argv[1]),
            enif_get_bool(env, argv[2]),
            enif_get_bool(env, argv[3])
        );
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
importFilter_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        main::api::corpus::importFilter(
            enif_get_bool(env, argv[0]),
            enif_get_bool(env, argv[1])
        );
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
importPattern_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        main::api::corpus::importPattern(enif_get_bool(env, argv[0]));
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
importTrain_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        main::api::corpus::importTrain(
            enif_get_bool(env, argv[0]),
            enif_get_bool(env, argv[1])
        );
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
importTriples_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        main::api::corpus::importTriples(
            subset::decodeName(enif_get_atom_(env, argv[0])),
            enif_get_bool(env, argv[1])
        );
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
importTypes_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        main::api::corpus::importTypes(
            enif_get_bool(env, argv[0])
        );
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
countEntities_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        return completed_with_success(
            env,
            enif_make_long(
                env,
                main::api::corpus::countEntities(
                    enif_get_bool(env, argv[0])
                )
            )
        );
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }
}

extern ERL_NIF_TERM
countRelations_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        return completed_with_success(
            env,
            enif_make_long(
                env,
                main::api::corpus::countRelations(
                    enif_get_bool(env, argv[0])
                )
            )
        );
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
countTriples_1(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        return completed_with_success(
            env,
            enif_make_long(
                env,
                main::api::corpus::countTriples(
                    enif_get_bool(env, argv[0])
                )
            )
        );
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
countTriples_2(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        return completed_with_success(
            env,
            enif_make_long(
                env,
                main::api::corpus::countTriples(
                    subset::decodeName(enif_get_atom_(env, argv[0])),
                    enif_get_bool(env, argv[1])
                )
            )
        );
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}
