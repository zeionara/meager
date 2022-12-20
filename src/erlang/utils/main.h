#ifndef ERLANG_UTILS_MAIN_H
#define ERLANG_UTILS_MAIN_H

#include "erl_nif.h"

#include "../../base/sampling/batch/Triple.h"

namespace meager::erlang::utils {

    template <typename T>
    struct List {
        T* items;
        long length;

        List(T* items, long length) {
            this->items = items;
            this->length = length;
        }
    };

    namespace encode {
        ERL_NIF_TERM tripleBatch(ErlNifEnv* env, main::sampling::batch::Triple* tripleBatch, bool encodeLabels = true);
    }

}

#endif
