#ifndef ERLANG_SETTING_H

#define ERLANG_SETTING_H
#include "erl_nif.h"

extern ERL_NIF_TERM
set_in_path(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
set_bern(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
set_work_threads(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
get_relation_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
get_entity_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
get_train_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
get_test_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

extern ERL_NIF_TERM
get_valid_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

#endif

