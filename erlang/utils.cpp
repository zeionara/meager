#include "erl_nif.h"
#include <iostream>
#include <string.h>

char* enif_get_string_(ErlNifEnv *env, ERL_NIF_TERM string, int length) {
    int decoded_length;

    enif_get_int(env, string, &decoded_length);
    decoded_length += 1;
    
    char* decoded_string = new char[decoded_length]();

    enif_get_string(env, string, decoded_string, decoded_length, ERL_NIF_LATIN1);

    return decoded_string;
}

char* enif_get_atom_(ErlNifEnv *env, ERL_NIF_TERM atom, int length) {
    int decoded_length;

    enif_get_int(env, atom, &decoded_length);
    decoded_length += 1;
    
    char* decoded_atom = new char[decoded_length]();

    enif_get_atom(env, atom, decoded_atom, decoded_length, ERL_NIF_LATIN1);

    return decoded_atom;
}

bool enif_get_bool(ErlNifEnv *env, ERL_NIF_TERM atom, int length) {
    char* decoded_atom = enif_get_atom_(env, atom, length);
    
    if (strcmp(decoded_atom, "true") == 0) {
        return true;
    } else if (strcmp(decoded_atom, "false") == 0) {
        return false;
    } else {
        printf("cannot recognize binary value of an atom %s\n\r", decoded_atom);
        throw std::invalid_argument(strcat((char *)"cannot recognize binary value of an atom ", decoded_atom));
    }
}

