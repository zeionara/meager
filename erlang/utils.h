#ifndef ERLANG_UTILS_H

#define ERLANG_UTILS_H
#include "erl_nif.h"
#include <stdbool.h>

#include <string>
#include <iostream>
#include <sstream>

extern
char* enif_get_string_(ErlNifEnv *env, ERL_NIF_TERM string, ERL_NIF_TERM length);

extern
char* enif_get_atom_(ErlNifEnv *env, ERL_NIF_TERM atom, ERL_NIF_TERM length);

extern
char* enif_get_atom_(ErlNifEnv *env, ERL_NIF_TERM atom);

extern
bool enif_get_bool(ErlNifEnv *env, ERL_NIF_TERM atom, ERL_NIF_TERM length);

extern
bool enif_get_bool(ErlNifEnv *env, ERL_NIF_TERM atom);

extern
int enif_get_int_(ErlNifEnv *env, ERL_NIF_TERM value);

extern
long enif_get_long_(ErlNifEnv *env, ERL_NIF_TERM value);

extern
void enif_encode_array_of_long(ErlNifEnv *env, long* source, ERL_NIF_TERM* destination, int length);

extern
void enif_encode_array_of_float(ErlNifEnv *env, float* source, ERL_NIF_TERM* destination, int length);

extern
void encode_array_of_float(ErlNifEnv *env, ERL_NIF_TERM source, float* destination, int length);

ERL_NIF_TERM completed_with_success(ErlNifEnv *env, ERL_NIF_TERM result);

ERL_NIF_TERM completed_with_error(ErlNifEnv *env, const char* result);

ERL_NIF_TERM completed_with_error(ErlNifEnv *env, std::stringstream* result);

ERL_NIF_TERM enif_make_string(ErlNifEnv *env, const char* result);

#endif

