#ifndef ERLANG_UTILS_NIF_ENCODE_H
#define ERLANG_UTILS_NIF_ENCODE_H

#include "erl_nif.h"

namespace meager::erlang::utils::nif::encode {

    extern void list(ErlNifEnv *env, long* source, ERL_NIF_TERM* destination, int length);
    extern void list(ErlNifEnv *env, float* source, ERL_NIF_TERM* destination, int length);

    ERL_NIF_TERM string(ErlNifEnv *env, const char* result);

}

#endif
