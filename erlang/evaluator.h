#ifndef ERLANG_EVALUATOR_H
#define ERLANG_EVALUATOR_H

#include "erl_nif.h"

extern ERL_NIF_TERM
initEvaluator_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
trial_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
evaluate_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
computeMetrics_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

#endif
