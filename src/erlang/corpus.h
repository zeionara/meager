#ifndef ERLANG_CORPUS_H
#define ERLANG_CORPUS_H

#include "erl_nif.h"

namespace meager::erlang::api::corpus {

    extern ERL_NIF_TERM
    init(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    importFilter(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    importPattern(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    importTrain(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    importTriples(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    importTypes(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    countEntities(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    countRelations(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

    extern ERL_NIF_TERM
    countTriples_1(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);  // 1 argument (verbose)

    extern ERL_NIF_TERM
    countTriples_2(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);  // 2 arguments (subset name, verbose)

}

#endif
