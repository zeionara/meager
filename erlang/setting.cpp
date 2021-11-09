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

