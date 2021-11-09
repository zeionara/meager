#include "erl_nif.h"
#include "Setting.h"
#include <iostream>

//
// Ancillary conversion functions
//

char* enif_get_string(ErlNifEnv *env, ERL_NIF_TERM string, int length) {
    int decoded_length;

    enif_get_int(env, string, &decoded_length);
    decoded_length += 1;
    
    char* decoded_string = new char[decoded_length]();

    enif_get_string(env, string, decoded_string, decoded_length, ERL_NIF_LATIN1);

    return decoded_string;
}

char* enif_get_atom(ErlNifEnv *env, ERL_NIF_TERM atom, int length) {
    int decoded_length;

    enif_get_int(env, atom, &decoded_length);
    decoded_length += 1;
    
    char* decoded_atom = new char[decoded_length]();

    enif_get_atom(env, atom, decoded_atom, decoded_length, ERL_NIF_LATIN1);

    return decoded_atom;
}

bool enif_get_bool(ErlNifEnv *env, ERL_NIF_TERM atom, int length) {
    char* decoded_atom = enif_get_atom(env, atom, length);
    
    if (strcmp(decoded_atom, "true") == 0) {
        return true;
    } else if (strcmp(decoded_atom, "false") == 0) {
        return false;
    } else {
        printf("cannot recognize binary value of an atom %s\n\r", decoded_atom);
        throw std::invalid_argument(strcat((char *)"cannot recognize binary value of an atom ", decoded_atom));
    }
}

//
// API Access functions
//

static ERL_NIF_TERM
fast_compare(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int a, b;
    // Fill a and b with the values of the first two args
    enif_get_int(env, argv[0], &a);
    enif_get_int(env, argv[1], &b);

    // Usual C unreadable code because this way is more true
    int result = a == b ? 0 : (a > b ? 1 : -1);

    return enif_make_int(env, result);
}

static ERL_NIF_TERM
set_in_path(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // int path_length, as_tsv_length;

    // enif_get_int(env, argv[2], &path_length);
    // path_length += 1;

    // enif_get_int(env, argv[3], &as_tsv_length);
    // as_tsv_length += 1;

    // char path[path_length];
    // char *as_tsv = new char[as_tsv_length]();

    // enif_get_string(env, argv[0], path, path_length, ERL_NIF_LATIN1);
    // printf("got path: '%s'\n\r", path);
    // for(int i = 0 ; i < path_length ; i ++ ){
    //     std::cout << path[i] ;//Looping 5 times to print out [0],[1],[2],[3],[4]
    // }
    // enif_get_atom(env, argv[1], as_tsv, as_tsv_length, ERL_NIF_LATIN1);
    // printf("got as-tsv: '%s'\n\r", as_tsv);

    // setInPath(path, strcmp(as_tsv, "true") == 0);
    setInPath(
        enif_get_string(env, argv[0], argv[2]),
        enif_get_bool(env, argv[1], argv[3])
    );

    // int result = 0;

    return enif_make_int(env, 0);
    
    // char* result = "result";

    // return enif_make_string(env, result, ERL_NIF_LATIN1);
}

// Let's define the array of ErlNifFunc beforehand:
// static ErlNifFunc fast_compare_nif_funcs[] = {
//     // {erl_function_name, erl_function_arity, c_function}
//     {"fast_compare", 2, fast_compare}
// };

static ErlNifFunc meager_nif_funcs[] = {
    // {erl_function_name, erl_function_arity, c_function}
    {"set_in_path", 4, set_in_path}
};

// ERL_NIF_INIT(Elixir.FastCompare, nif_funcs, NULL, NULL, NULL, NULL)
// ERL_NIF_INIT(Elixir.FastCompare, fast_compare_nif_funcs, NULL, NULL, NULL, NULL)
// ERL_NIF_INIT(Elixir.FastCompare, fast_compare_nif_funcs, NULL, NULL, NULL, NULL)
ERL_NIF_INIT(Elixir.Meager, meager_nif_funcs, NULL, NULL, NULL, NULL)

