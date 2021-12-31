#include "erl_nif.h"
#include "utils.h"
#include "../base/Setting.h"

extern ERL_NIF_TERM
set_in_path(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    setInPath(
        enif_get_string_(env, argv[0], argv[2]),
        enif_get_bool(env, argv[1], argv[3])
    );

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
set_bern(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    setBern(
        enif_get_int_(env, argv[0])
    );

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
set_work_threads(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    setWorkThreads(
        enif_get_int_(env, argv[0])
    );

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
get_relation_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, getRelationTotal());
}

extern ERL_NIF_TERM
get_entity_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, getEntityTotal());
}

extern ERL_NIF_TERM
get_train_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, getTrainTotal());
}

extern ERL_NIF_TERM
get_test_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, getTestTotal());
}

extern ERL_NIF_TERM
get_valid_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, getValidTotal());
}
