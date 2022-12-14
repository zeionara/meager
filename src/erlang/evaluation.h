#ifndef ERLANG_EVALUATION_H
#define ERLANG_EVALUATION_H

#include "erl_nif.h"

namespace meager::erlang::api::evaluation {

    extern ERL_NIF_TERM
    init(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    trial(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    evaluate(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    computeMetrics(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

}

#endif
