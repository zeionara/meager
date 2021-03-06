#include "erl_nif.h"
#include "utils.h"
#include "../base/Reader.h"

extern ERL_NIF_TERM
import_train_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    importTrainFiles(
        enif_get_bool(env, argv[0], argv[1])
    );

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
import_test_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    importTestFiles(
        enif_get_bool(env, argv[0], argv[1])
    );

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
import_type_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    importTypeFiles();

    return enif_make_int(env, 0);
}

