#include "erl_nif.h"
#include "utils.h"
// #include "../base/Reader.h"
#include "../base/filters/main.h"

#include "../base/global.h"
#include "../base/api/read.h"

extern ERL_NIF_TERM
import_filter_patterns(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // importFilterPatterns(
    //     enif_get_bool(env, argv[0]),
    //     enif_get_bool(env, argv[1]),
    //     enif_get_bool(env, argv[2])
    // );

    importFilter(
        enif_get_bool(env, argv[0]),
        enif_get_bool(env, argv[1])
    );

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
import_train_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // importTrainFiles(
    //     enif_get_bool(env, argv[0], argv[1]),
    //     enif_get_bool(env, argv[2])
    // );

    importTrain(enif_get_bool(env, argv[0], argv[1]));

    // corpus->importTrain(
    //     getInputPath(triples, train),
    //     enif_get_bool(env, argv[2]),
    //     enif_get_bool(env, argv[0], argv[1])
    // );

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
import_test_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // importTestFiles(
    //     enif_get_bool(env, argv[0], argv[1]),
    //     enif_get_bool(env, argv[2])
    // );

    importTest(enif_get_bool(env, argv[0], argv[1]));
    importValid(enif_get_bool(env, argv[0], argv[1]));

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
import_type_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // importTypeFiles();

    importTypes(true);

    return enif_make_int(env, 0);
}

