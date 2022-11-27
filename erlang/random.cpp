#include "erl_nif.h"
#include "utils.h"
#include "../base/Random.h"

extern ERL_NIF_TERM
rand_reset(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    randReset(4);

    return enif_make_int(env, 0);
}

