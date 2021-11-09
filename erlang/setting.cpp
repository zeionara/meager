#include "erl_nif.h"
#include "utils.h"
#include "../base/Setting.h"
// #include "../base/utils.h"

// bool enif_get_bool_(ErlNifEnv *env, ERL_NIF_TERM atom, int length) {
//     // char* decoded_atom = enif_get_atom_(env, atom, length);
//     // 
//     // if (strcmp(decoded_atom, "true") == 0) {
//     //     return true;
//     // } else if (strcmp(decoded_atom, "false") == 0) {
//     //     return false;
//     // } else {
//     //     printf("cannot recognize binary value of an atom %s\n\r", decoded_atom);
//     //     throw std::invalid_argument(strcat((char *)"cannot recognize binary value of an atom ", decoded_atom));
//     // }
// 
//     return false;
// }

extern ERL_NIF_TERM
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
        enif_get_string_(env, argv[0], argv[2]),
        enif_get_bool(env, argv[1], argv[3])
    );

    // int result = 0;

    return enif_make_int(env, 0);
    
    // char* result = "result";

    // return enif_make_string(env, result, ERL_NIF_LATIN1);
}

