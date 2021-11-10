#ifndef ERLANG_RANDOM_H

#define ERLANG_RANDOM_H
#include "erl_nif.h"

extern ERL_NIF_TERM
rand_reset(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

#endif

