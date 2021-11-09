#include "erl_nif.h"
// #include <iostream>

#include "setting.h"
#include "utils.h"

//
// Ancillary conversion functions
//


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

