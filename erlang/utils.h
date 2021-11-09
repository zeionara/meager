#ifndef ERLANG_UTILS_H

#define ERLANG_UTILS_H
#include "erl_nif.h"

extern
char* enif_get_string_(ErlNifEnv *env, ERL_NIF_TERM string, int length);

extern
char* enif_get_atom_(ErlNifEnv *env, ERL_NIF_TERM atom, int length);

extern
bool enif_get_bool(ErlNifEnv *env, ERL_NIF_TERM atom, int length);

extern
bool enif_get_int_(ErlNifEnv *env, ERL_NIF_TERM atom);

#endif

