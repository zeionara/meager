#include "erl_nif.h"
#include "setting.h"
#include "random.h"
#include "reader.h"
#include "../base/Setting.h"
#include "utils.h"
#include "../base/main.h"
#include <iostream>

extern ERL_NIF_TERM
sample(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    INT batch_size = enif_get_long_(env, argv[0]); 
    INT entity_negative_rate = enif_get_long_(env, argv[1]); 
    INT relation_negative_rate = enif_get_long_(env, argv[2]); 
    INT head_batch_flag = 0;
    
    if (enif_get_bool(env, argv[3], argv[4])) {
        head_batch_flag = 1;
    }

    int batch_tuple_size = batch_size * (1 + entity_negative_rate + relation_negative_rate);

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_y = new ERL_NIF_TERM[batch_tuple_size]();

    INT* batch_h_encoded = new INT[batch_tuple_size]();
    INT* batch_t_encoded = new INT[batch_tuple_size]();
    INT* batch_r_encoded = new INT[batch_tuple_size]();
    REAL* batch_y_encoded = new REAL[batch_tuple_size]();

    // std::cout << "Sampling...\n";

    sampling(batch_h_encoded, batch_t_encoded, batch_r_encoded, batch_y_encoded, batch_size, entity_negative_rate, relation_negative_rate, head_batch_flag);

    // std::cout << "Sampled\n";

    enif_encode_array_of_long(env, batch_h_encoded, batch_h, batch_tuple_size);
    enif_encode_array_of_long(env, batch_t_encoded, batch_t, batch_tuple_size);
    enif_encode_array_of_long(env, batch_r_encoded, batch_r, batch_tuple_size);
    enif_encode_array_of_float(env, batch_y_encoded, batch_y, batch_tuple_size);

    delete [] batch_h_encoded;
    delete [] batch_t_encoded;
    delete [] batch_r_encoded;
    delete [] batch_y_encoded;

    ERL_NIF_TERM* batch = new ERL_NIF_TERM[4]();

    batch[0] = enif_make_list_from_array(env, batch_h, batch_tuple_size);
    batch[1] = enif_make_list_from_array(env, batch_t, batch_tuple_size);
    batch[2] = enif_make_list_from_array(env, batch_r, batch_tuple_size);
    batch[3] = enif_make_list_from_array(env, batch_y, batch_tuple_size);

    delete [] batch_h;
    delete [] batch_t;
    delete [] batch_r;
    delete [] batch_y;
    
    ERL_NIF_TERM result = enif_make_list_from_array(env, batch, 4);

    delete [] batch;

    return result;

    // ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[2]();

    // batch_h[0] = enif_make_int(env, 17);
    // batch_h[1] = enif_make_int(env, 18);

    // return enif_make_list_from_array(env, batch_h, 2);

    // const ERL_NIF_TERM* batch_h;
    // int batch_h_size;

    // enif_get_tuple(env, argv[0], &batch_h_size, &batch_h);
    // 
    // printf("N elements in tuple: %d\n\r", batch_h_size); 

    // return enif_make_int(env, 0);
}

static ErlNifFunc meager_nif_funcs[] = {
    //
    //  Settings
    //  

    {"set_in_path", 4, set_in_path},
    {"set_bern", 1, set_bern},
    {"set_work_threads", 1, set_work_threads},

    {"get_relation_total", 0, get_relation_total},
    {"get_entity_total", 0, get_entity_total},
    {"get_train_total", 0, get_train_total},
    {"get_test_total", 0, get_test_total},
    {"get_valid_total", 0, get_valid_total},
    
    //
    //  Random
    // 

    {"rand_reset", 0, rand_reset},

    // 
    //  Reader
    //  

    {"import_train_files", 2, import_train_files},
    {"import_test_files", 2, import_test_files},
    {"import_type_files", 0, import_type_files},

    //
    //  Sampling
    //

    {"sample", 5, sample}
};

ERL_NIF_INIT(Elixir.Meager, meager_nif_funcs, NULL, NULL, NULL, NULL)

