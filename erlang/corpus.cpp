#include "erl_nif.h"
#include "utils.h"

#include "../base/api/corpus.h"

extern ERL_NIF_TERM
initCorpus_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        initCorpus(
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
        importFilter(
            enif_get_bool(env, argv[0]),
            enif_get_bool(env, argv[1])
        );
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
importTrain_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        importTrain(enif_get_bool(env, argv[0]));
    } catch (invalid_argument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}
