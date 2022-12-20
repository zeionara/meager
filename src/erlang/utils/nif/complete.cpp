#include "sstream"

#include "erl_nif.h"

#include "encode.h"

using namespace std;

namespace meager::erlang::utils::nif::complete {

    ERL_NIF_TERM complete(ErlNifEnv *env, char* status, ERL_NIF_TERM result) {
        return enif_make_tuple2(
            env,
            enif_make_atom(env, status),
            result
        );
    }

    ERL_NIF_TERM success(ErlNifEnv *env, ERL_NIF_TERM result) {
        return complete(env, (char*)"ok", result);
    }

    ERL_NIF_TERM success(ErlNifEnv *env) {
        return complete(env, (char*)"ok", enif_make_atom(env, "nil"));
    }

    ERL_NIF_TERM error(ErlNifEnv *env, const char* result) {
        return complete(env, (char*)"error", encode::string(env, result));
    }

    ERL_NIF_TERM error(ErlNifEnv *env, stringstream* result) {
        return error(env, result->str().c_str());
    }

}
