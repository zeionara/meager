#ifndef ERLANG_SAMPLER_H
#define ERLANG_SAMPLER_H

#include "erl_nif.h"
#include "../base/samplers/TripleBatch.h"

extern ERL_NIF_TERM
initSampler_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
sample_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
encodeTripleBatch(ErlNifEnv* env, TripleBatch* tripleBatch, bool encodeLabels = true);

#endif
