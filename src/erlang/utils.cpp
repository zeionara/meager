#include "erl_nif.h"
#include <iostream>
#include <string.h>

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

char* enif_get_string_(ErlNifEnv *env, ERL_NIF_TERM string, ERL_NIF_TERM length) {
    int decoded_length;

    enif_get_int(env, length, &decoded_length);
    decoded_length += 1;
    
    char* decoded_string = new char[decoded_length]();

    enif_get_string(env, string, decoded_string, decoded_length, ERL_NIF_LATIN1);

    return decoded_string;
}

char* enif_get_atom_(ErlNifEnv *env, ERL_NIF_TERM atom, ERL_NIF_TERM length) {
    int decoded_length;

    enif_get_int(env, length, &decoded_length);
    decoded_length += 1;
    
    char* decoded_atom = new char[decoded_length]();

    enif_get_atom(env, atom, decoded_atom, decoded_length, ERL_NIF_LATIN1);

    return decoded_atom;
}

char* enif_get_atom_(ErlNifEnv *env, ERL_NIF_TERM atom) {
    unsigned int decoded_length;

    enif_get_atom_length(env, atom, &decoded_length, ERL_NIF_LATIN1);
    decoded_length += 1;
    
    char* decoded_atom = new char[decoded_length]();

    enif_get_atom(env, atom, decoded_atom, decoded_length, ERL_NIF_LATIN1);

    return decoded_atom;
}

long getListLength(ErlNifEnv* env, ERL_NIF_TERM list) {
    unsigned length;
    enif_get_list_length(env, list, &length);
    return length;
}

bool enif_get_bool(ErlNifEnv *env, ERL_NIF_TERM atom, ERL_NIF_TERM length) {
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

bool enif_get_bool(ErlNifEnv *env, ERL_NIF_TERM atom) {
    // unsigned int decoded_length;

    // cout << "getting atom length..." << endl;
    // enif_get_atom_length(env, atom, &decoded_length, ERL_NIF_LATIN1);
    // cout << "got atom length" << endl;
    // decoded_length += 1;

    // cout << "getting atom..." << endl;
    // cout << decoded_length << endl;
    char* decoded_atom = enif_get_atom_(env, atom);
    // cout << "got atom" << endl;
    
    if (strcmp(decoded_atom, "true") == 0) {
        return true;
    } else if (strcmp(decoded_atom, "false") == 0) {
        return false;
    } else {
        printf("cannot recognize binary value of an atom %s\n\r", decoded_atom);
        throw std::invalid_argument(strcat((char *)"cannot recognize binary value of an atom ", decoded_atom));
    }
}

int enif_get_int_(ErlNifEnv *env, ERL_NIF_TERM value) {
    int decoded_value; 
    
    enif_get_int(env, value, &decoded_value);

    return decoded_value;
}

long enif_get_long_(ErlNifEnv *env, ERL_NIF_TERM value) {
    long decoded_value; 
    
    enif_get_long(env, value, &decoded_value);

    return decoded_value;
}

void enif_encode_array_of_long(ErlNifEnv* env, long* source, ERL_NIF_TERM* destination, int length) {
    for (int i = 0; i < length; i++) {
        destination[i] = enif_make_long(env, source[i]);
    }
}

void enif_encode_array_of_float(ErlNifEnv* env, float* source, ERL_NIF_TERM* destination, int length) {
    for (int i = 0; i < length; i++) {
        destination[i] = enif_make_double(env, source[i]);
    }
}

void encode_array_of_float(ErlNifEnv *env, ERL_NIF_TERM source, float* destination, int length) {
    ERL_NIF_TERM head;
    ERL_NIF_TERM tail = source;

    double current_value;

    for (int i = 0; i < length; i++) {
        enif_get_list_cell(env, tail, &head, &tail);
        enif_get_double(env, head, &current_value);
        destination[i] = current_value;
    }
}

ERL_NIF_TERM enif_make_string(ErlNifEnv *env, const char* string) {
    return enif_make_string(env, string, ERL_NIF_LATIN1);
}

ERL_NIF_TERM make_result(ErlNifEnv *env, char* status, ERL_NIF_TERM result) {
    return enif_make_tuple2(
        env,
        enif_make_atom(env, status),
        result
    );
}

ERL_NIF_TERM completed_with_success(ErlNifEnv *env, ERL_NIF_TERM result) {
    return make_result(env, (char*)"ok", result);
}

ERL_NIF_TERM completed_with_success(ErlNifEnv *env) {
    return make_result(env, (char*)"ok", enif_make_atom(env, "nil"));
}

ERL_NIF_TERM completed_with_error(ErlNifEnv *env, const char* result) {
    return make_result(env, (char*)"error", enif_make_string(env, result));
}

ERL_NIF_TERM completed_with_error(ErlNifEnv *env, stringstream* result) {
    return completed_with_error(env, result->str().c_str());
}
