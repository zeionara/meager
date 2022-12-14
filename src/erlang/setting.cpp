#include "erl_nif.h"
#include "utils.h"
// #include "../base/Setting.h"
#include "../base/api/corpus.h"

// using namespace meager::main::utils;
using namespace meager;

extern ERL_NIF_TERM
set_in_path(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    main::api::corpus::init(
        enif_get_string_(env, argv[0], argv[2])
    );

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
set_bern(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // setBern(
    //     enif_get_int_(env, argv[0])
    // );

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
set_work_threads(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // setWorkThreads(
    //     enif_get_int_(env, argv[0])
    // );

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
get_relation_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, main::api::corpus::countRelations());
}

extern ERL_NIF_TERM
get_entity_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, main::api::corpus::countEntities());
}

extern ERL_NIF_TERM
get_train_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, main::api::corpus::countTriples(subset::Type::train));
}

extern ERL_NIF_TERM
get_test_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, main::api::corpus::countTriples(subset::Type::test));
}

extern ERL_NIF_TERM
get_valid_total(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_int(env, main::api::corpus::countTriples(subset::Type::valid));
}

extern ERL_NIF_TERM
set_head_tail_cross_sampling(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]){
    // setHeadTailCrossSampling(
    //     enif_get_int_(env, argv[0])
    // );

    return enif_make_int(env, 0);
}
