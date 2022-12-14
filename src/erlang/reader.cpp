#include "erl_nif.h"
#include "utils.h"
// #include "../base/Reader.h"
// #include "../base/filters/main.h"

// #include "../base/global.h"
// #include "../base/api/read.h"
#include "../base/api/corpus.h"

// using namespace meager::main::utils;
using namespace meager;

extern ERL_NIF_TERM
import_filter_patterns(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // importFilterPatterns(
    //     enif_get_bool(env, argv[0]),
    //     enif_get_bool(env, argv[1]),
    //     enif_get_bool(env, argv[2])
    // );

    main::api::corpus::importFilter(
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

    main::api::corpus::importTrain(enif_get_bool(env, argv[0], argv[1]));

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

    main::api::corpus::importTriples(subset::Type::test, enif_get_bool(env, argv[0], argv[1]));
    main::api::corpus::importTriples(subset::Type::valid, enif_get_bool(env, argv[0], argv[1]));

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
import_type_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // importTypeFiles();

    main::api::corpus::importTypes(true);

    return enif_make_int(env, 0);
}

