#include "erl_nif.h"
#include "utils.h"
#include "../base/Setting.h"
#include "../base/Test.h"

//
//  Test
//

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

    REAL* predictions_encoded = new REAL[batch_tuple_size]();

    encode_array_of_float(env, argv[0], predictions_encoded, batch_tuple_size);
    
    testHead(predictions_encoded, enif_get_bool(env, argv[1]));

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

    testTail(predictions_encoded, enif_get_bool(env, argv[1]));

    return enif_make_int(env, 0);
}

//
//  Validate
//


extern ERL_NIF_TERM
get_valid_head_batch(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = entityTotal;

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batch_tuple_size]();

    INT* batch_h_encoded = new INT[batch_tuple_size]();
    INT* batch_t_encoded = new INT[batch_tuple_size]();
    INT* batch_r_encoded = new INT[batch_tuple_size]();

    getValidHeadBatch(batch_h_encoded, batch_t_encoded, batch_r_encoded);

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
valid_head(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = entityTotal;

    REAL* predictions_encoded = new REAL[batch_tuple_size]();

    encode_array_of_float(env, argv[0], predictions_encoded, batch_tuple_size);
    
    validHead(predictions_encoded, enif_get_bool(env, argv[1]));

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
get_valid_tail_batch(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = entityTotal;

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batch_tuple_size]();

    INT* batch_h_encoded = new INT[batch_tuple_size]();
    INT* batch_t_encoded = new INT[batch_tuple_size]();
    INT* batch_r_encoded = new INT[batch_tuple_size]();

    getValidTailBatch(batch_h_encoded, batch_t_encoded, batch_r_encoded);

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
valid_tail(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = entityTotal;

    REAL* predictions_encoded = new REAL[batch_tuple_size]();

    encode_array_of_float(env, argv[0], predictions_encoded, batch_tuple_size);

    validTail(predictions_encoded, enif_get_bool(env, argv[1]));

    return enif_make_int(env, 0);
}

ERL_NIF_TERM test_link_prediction_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    test_link_prediction(
        enif_get_bool(env, argv[0], argv[1])
    );

    return enif_make_int(env, 0);
}

ERL_NIF_TERM init_test(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    initTest();

    return enif_make_int(env, 0);
}

