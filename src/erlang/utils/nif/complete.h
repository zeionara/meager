#ifndef ERLANG_UTILS_NIF_COMPLETE_H
#define ERLANG_UTILS_NIF_COMPLETE_H

#include "sstream"

#include "erl_nif.h"

namespace meager::erlang::utils::nif::complete {

    ERL_NIF_TERM success(ErlNifEnv *env, ERL_NIF_TERM result);
    ERL_NIF_TERM success(ErlNifEnv *env);

    ERL_NIF_TERM error(ErlNifEnv *env, const char* result);
    ERL_NIF_TERM error(ErlNifEnv *env, std::stringstream* result);

}

#endif
