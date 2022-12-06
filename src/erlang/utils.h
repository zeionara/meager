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

ERL_NIF_TERM completed_with_success(ErlNifEnv *env);

ERL_NIF_TERM completed_with_error(ErlNifEnv *env, const char* result);

ERL_NIF_TERM completed_with_error(ErlNifEnv *env, std::stringstream* result);

ERL_NIF_TERM enif_make_string(ErlNifEnv *env, const char* result);

template <typename T>
void decodeList(ErlNifEnv *env, ERL_NIF_TERM encoded, T* decoded, int length, T (*postProcess)(ErlNifEnv*, ERL_NIF_TERM)) {
    ERL_NIF_TERM head;
    ERL_NIF_TERM tail = encoded;

    double current_value;

    for (int i = 0; i < length; i++) {
        enif_get_list_cell(env, tail, &head, &tail);  // Fetch next element from tail and save it to head
        decoded[i] = postProcess(env, head);
    }
}

long getListLength(ErlNifEnv* env, ERL_NIF_TERM list);

template <typename T>
struct List {
    T* items;
    long length;

    List(T* items, long length) {
        this->items = items;
        this->length = length;
    }
};

template <typename T>
List<T>* decodeList(ErlNifEnv *env, ERL_NIF_TERM encoded, T (*postProcess)(ErlNifEnv*, ERL_NIF_TERM)) {
    ERL_NIF_TERM head;
    ERL_NIF_TERM tail = encoded;

    long length = getListLength(env, encoded);

    T* decoded = (T*)malloc(length * sizeof(T));

    double current_value;

    for (int i = 0; i < length; i++) {
        enif_get_list_cell(env, tail, &head, &tail);  // Fetch next element from tail and save it to head
        decoded[i] = postProcess(env, head);
    }

    return new List<T>(decoded, length);
}


#endif

