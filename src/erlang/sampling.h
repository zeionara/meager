#ifndef ERLANG_SAMPLING_H
#define ERLANG_SAMPLING_H

#include "erl_nif.h"
#include "../base/sampling/batch/Triple.h"

namespace meager::erlang::api::sampling {

    extern ERL_NIF_TERM
    init(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    sample(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    encodeTripleBatch(ErlNifEnv* env, meager::main::sampling::batch::Triple* tripleBatch, bool encodeLabels = true);

}

#endif
