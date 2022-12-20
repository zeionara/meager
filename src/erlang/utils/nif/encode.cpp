#include "erl_nif.h"

namespace meager::erlang::utils::nif::encode {

    void list(ErlNifEnv* env, long* source, ERL_NIF_TERM* destination, int length) {
        for (int i = 0; i < length; i++) {
            destination[i] = enif_make_long(env, source[i]);
        }
    }

    void list(ErlNifEnv* env, float* source, ERL_NIF_TERM* destination, int length) {
        for (int i = 0; i < length; i++) {
            destination[i] = enif_make_double(env, source[i]);
        }
    }


    ERL_NIF_TERM string(ErlNifEnv *env, const char* string) {
        return enif_make_string(env, string, ERL_NIF_LATIN1);
    }

}
