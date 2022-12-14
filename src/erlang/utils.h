#ifndef ERLANG_UTILS_H

#define ERLANG_UTILS_H
#include "erl_nif.h"
// #include <stdbool.h>

#include <string>
#include <iostream>
#include <sstream>

namespace meager::erlang::utils::nif {

    namespace decode {

        extern char* string(ErlNifEnv *env, ERL_NIF_TERM string, ERL_NIF_TERM length);

        [[deprecated]]
        extern char* atom(ErlNifEnv *env, ERL_NIF_TERM atom, ERL_NIF_TERM length);
        extern char* atom(ErlNifEnv *env, ERL_NIF_TERM atom);

        [[deprecated]]
        extern bool boolean(ErlNifEnv *env, ERL_NIF_TERM atom, ERL_NIF_TERM length);
        extern bool boolean(ErlNifEnv *env, ERL_NIF_TERM atom);

        extern int integer(ErlNifEnv *env, ERL_NIF_TERM value);
        extern long longInteger(ErlNifEnv *env, ERL_NIF_TERM value);

        template <typename T>
        struct List {
            T* items;
            long length;

            List(T* items, long length) {
                this->items = items;
                this->length = length;
            }
        };

        extern void list(ErlNifEnv *env, ERL_NIF_TERM source, float* destination, int length);

        long length(ErlNifEnv* env, ERL_NIF_TERM list);

        template <typename T>
        void list(ErlNifEnv *env, ERL_NIF_TERM encoded, T* decoded, int length, T (*postProcess)(ErlNifEnv*, ERL_NIF_TERM)) {
            ERL_NIF_TERM head;
            ERL_NIF_TERM tail = encoded;

            for (int i = 0; i < length; i++) {
                enif_get_list_cell(env, tail, &head, &tail);  // Fetch next element from tail and save it to head
                decoded[i] = postProcess(env, head);
            }
        }

        template <typename T>
        List<T>* list(ErlNifEnv *env, ERL_NIF_TERM encoded, T (*postProcess)(ErlNifEnv*, ERL_NIF_TERM)) {
            ERL_NIF_TERM head;
            ERL_NIF_TERM tail = encoded;

            long length = decode::length(env, encoded);

            T* decoded = (T*)malloc(length * sizeof(T));

            for (int i = 0; i < length; i++) {
                enif_get_list_cell(env, tail, &head, &tail);  // Fetch next element from tail and save it to head
                decoded[i] = postProcess(env, head);
            }

            return new List<T>(decoded, length);
        }
    }

    namespace encode {

        extern void list(ErlNifEnv *env, long* source, ERL_NIF_TERM* destination, int length);
        extern void list(ErlNifEnv *env, float* source, ERL_NIF_TERM* destination, int length);

        ERL_NIF_TERM string(ErlNifEnv *env, const char* result);

    }

    namespace complete {

        ERL_NIF_TERM success(ErlNifEnv *env, ERL_NIF_TERM result);
        ERL_NIF_TERM success(ErlNifEnv *env);

        ERL_NIF_TERM error(ErlNifEnv *env, const char* result);
        ERL_NIF_TERM error(ErlNifEnv *env, std::stringstream* result);

    }
}

#endif
