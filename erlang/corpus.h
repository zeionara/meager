#ifndef ERLANG_CORPUS_H
#define ERLANG_CORPUS_H

#include "erl_nif.h"

extern ERL_NIF_TERM
initCorpus_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
importFilter_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
importPattern_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
importTrain_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

#endif
