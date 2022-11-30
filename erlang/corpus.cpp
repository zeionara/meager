#include "erl_nif.h"
#include "utils.h"

#include "../base/api/corpus.h"

extern ERL_NIF_TERM
initCorpus_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    initCorpus(
        enif_get_string_(env, argv[0], argv[1]),
        enif_get_bool(env, argv[2]),
        enif_get_bool(env, argv[3])
    );

    return completed_with_success(env);
}

extern ERL_NIF_TERM
importFilter_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    importFilter(
        enif_get_bool(env, argv[0]),
        enif_get_bool(env, argv[1])
    );

    return completed_with_success(env);
}

extern ERL_NIF_TERM
importTrain_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    importTrain(enif_get_bool(env, argv[0]));

    return completed_with_success(env);
}
