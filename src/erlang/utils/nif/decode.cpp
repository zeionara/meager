#include "erl_nif.h"

#include <string.h>
// #include <iostream>
#include <sstream>

using namespace std;

namespace meager::erlang::utils::nif::decode {
    
    char* string(ErlNifEnv *env, ERL_NIF_TERM string, ERL_NIF_TERM length) {
        int decoded_length;

        enif_get_int(env, length, &decoded_length);
        decoded_length += 1;
        
        char* decoded_string = new char[decoded_length]();

        enif_get_string(env, string, decoded_string, decoded_length, ERL_NIF_LATIN1);

        return decoded_string;
    }

    char* atom(ErlNifEnv *env, ERL_NIF_TERM atom, ERL_NIF_TERM length) {
        int decoded_length;

        enif_get_int(env, length, &decoded_length);
        decoded_length += 1;
        
        char* decoded_atom = new char[decoded_length]();

        enif_get_atom(env, atom, decoded_atom, decoded_length, ERL_NIF_LATIN1);

        return decoded_atom;
    }

    char* atom(ErlNifEnv *env, ERL_NIF_TERM atom) {
        unsigned int decoded_length;

        enif_get_atom_length(env, atom, &decoded_length, ERL_NIF_LATIN1);
        decoded_length += 1;
        
        char* decoded_atom = new char[decoded_length]();

        enif_get_atom(env, atom, decoded_atom, decoded_length, ERL_NIF_LATIN1);

        return decoded_atom;
    }

    long length(ErlNifEnv* env, ERL_NIF_TERM list) {
        unsigned length;
        enif_get_list_length(env, list, &length);
        return length;
    }

    bool boolean(char* decoded_atom) {
        if (strcmp(decoded_atom, "true") == 0) {
            return true;
        } else if (strcmp(decoded_atom, "false") == 0) {
            return false;
        } else {
            printf("cannot recognize binary value of an atom %s\n\r", decoded_atom);
            throw std::invalid_argument(strcat((char *)"cannot recognize binary value of an atom ", decoded_atom));
        }
    }

    bool boolean(ErlNifEnv *env, ERL_NIF_TERM atom, ERL_NIF_TERM length) {
        char* decoded_atom = decode::atom(env, atom, length);
        
        return boolean(decoded_atom);
    }

    bool boolean(ErlNifEnv *env, ERL_NIF_TERM atom) {
        char* decoded_atom = decode::atom(env, atom);
        
        return boolean(decoded_atom);
    }

    int integer(ErlNifEnv *env, ERL_NIF_TERM value) {
        int decoded_value; 
        
        enif_get_int(env, value, &decoded_value);

        return decoded_value;
    }

    long longInteger(ErlNifEnv *env, ERL_NIF_TERM value) {
        long decoded_value; 
        
        enif_get_long(env, value, &decoded_value);

        return decoded_value;
    }

    void list(ErlNifEnv *env, ERL_NIF_TERM source, float* destination, int length) {
        ERL_NIF_TERM head;
        ERL_NIF_TERM tail = source;

        double current_value;

        for (int i = 0; i < length; i++) {
            enif_get_list_cell(env, tail, &head, &tail);
            enif_get_double(env, head, &current_value);
            destination[i] = current_value;
        }
    }

}
