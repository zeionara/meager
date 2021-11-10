#include "erl_nif.h"
#include "utils.h"
#include "../base/Setting.h"
#include "../base/Test.h"

extern ERL_NIF_TERM
get_head_batch(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = entityTotal;

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batch_tuple_size]();

    INT* batch_h_encoded = new INT[batch_tuple_size]();
    INT* batch_t_encoded = new INT[batch_tuple_size]();
    INT* batch_r_encoded = new INT[batch_tuple_size]();

    getHeadBatch(batch_h_encoded, batch_t_encoded, batch_r_encoded);

    enif_encode_array_of_long(env, batch_h_encoded, batch_h, batch_tuple_size);
    enif_encode_array_of_long(env, batch_t_encoded, batch_t, batch_tuple_size);
    enif_encode_array_of_long(env, batch_r_encoded, batch_r, batch_tuple_size);

    delete [] batch_h_encoded;
    delete [] batch_t_encoded;
    delete [] batch_r_encoded;

    ERL_NIF_TERM* batch = new ERL_NIF_TERM[3]();

    batch[0] = enif_make_list_from_array(env, batch_h, batch_tuple_size);
    batch[1] = enif_make_list_from_array(env, batch_t, batch_tuple_size);
    batch[2] = enif_make_list_from_array(env, batch_r, batch_tuple_size);

    delete [] batch_h;
    delete [] batch_t;
    delete [] batch_r;

    ERL_NIF_TERM result = enif_make_list_from_array(env, batch, 3);

    delete [] batch;

    return result;
}

extern ERL_NIF_TERM
test_head(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = entityTotal;

    // ERL_NIF_TERM* predictions = new ERL_NIF_TERM[batch_tuple_size]();
    REAL* predictions_encoded = new REAL[batch_tuple_size]();

    // ERL_NIF_TERM head;
    // ERL_NIF_TERM tail = argv[0];

    // enif_get_list_cell(env, argv[0], &head, &tail);

    // const ERL_NIF_TERM* pair;
    // const ERL_NIF_TERM* another_pair;
    // int pair_arity;
    // double probability;
    // double prob;

    // printf("Pair arity: %d\n", pair_arity);
    // enif_get_tuple(env, head, &pair_arity, &pair);
    // printf("Pair arity: %d\n", pair_arity);

    // for (int i = 0; i < batch_tuple_size; i++) {
    //     enif_get_list_cell(env, tail, &head, &tail);
    //     enif_get_double(env, head, &probability);
    //     predictions_encoded[i] = probability;
    //     printf("Prob: %f\n", predictions_encoded[i]);
    // }
    
    // enif_get_double(env, head, &probability);
    // printf("Prob: %f\n", probability);
 
    // enif_get_list_cell(env, tail, &head, &tail);
    // enif_get_double(env, head, &prob);
    // printf("Prob: %f\n", prob);

    // enif_get_tuple(env, pair[1], &pair_arity, &another_pair);
    // enif_get_double(env, pair[0], &probability);

    encode_array_of_float(env, argv[0], predictions_encoded, batch_tuple_size);
    
    // for (int i = 0; i < batch_tuple_size; i++) {
    //     printf("Prob: %f\n", predictions_encoded[i]);
    // }

    testHead(predictions_encoded);

    // delete [] predictions;
    // delete [] predictions_encoded;

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
get_tail_batch(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = entityTotal;

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batch_tuple_size]();

    INT* batch_h_encoded = new INT[batch_tuple_size]();
    INT* batch_t_encoded = new INT[batch_tuple_size]();
    INT* batch_r_encoded = new INT[batch_tuple_size]();

    getTailBatch(batch_h_encoded, batch_t_encoded, batch_r_encoded);

    enif_encode_array_of_long(env, batch_h_encoded, batch_h, batch_tuple_size);
    enif_encode_array_of_long(env, batch_t_encoded, batch_t, batch_tuple_size);
    enif_encode_array_of_long(env, batch_r_encoded, batch_r, batch_tuple_size);

    delete [] batch_h_encoded;
    delete [] batch_t_encoded;
    delete [] batch_r_encoded;

    ERL_NIF_TERM* batch = new ERL_NIF_TERM[3]();

    batch[0] = enif_make_list_from_array(env, batch_h, batch_tuple_size);
    batch[1] = enif_make_list_from_array(env, batch_t, batch_tuple_size);
    batch[2] = enif_make_list_from_array(env, batch_r, batch_tuple_size);

    delete [] batch_h;
    delete [] batch_t;
    delete [] batch_r;

    ERL_NIF_TERM result = enif_make_list_from_array(env, batch, 3);

    delete [] batch;

    return result;
}

extern ERL_NIF_TERM
test_tail(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = entityTotal;

    REAL* predictions_encoded = new REAL[batch_tuple_size]();

    encode_array_of_float(env, argv[0], predictions_encoded, batch_tuple_size);

    testTail(predictions_encoded);

    return enif_make_int(env, 0);
}

