#ifndef ERLANG_READER_H

#define ERLANG_READER_H
#include "erl_nif.h"

extern ERL_NIF_TERM
import_filter_patterns(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
import_train_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
import_test_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
import_type_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

#endif

