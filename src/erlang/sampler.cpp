#include "erl_nif.h"
#include "utils.h"

#include "../base/api/sampler.h"

ERL_NIF_TERM encodeTripleBatch(ErlNifEnv* env, sampling::batch::Triple* tripleBatch, bool verbose = false);

extern ERL_NIF_TERM
initSampler_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        triple::pattern::Pattern pattern = triple::pattern::decodeName(enif_get_atom_(env, argv[0]));

        initSampler(
            pattern,  // pattern
            enif_get_long_(env, argv[1]),  // nObservedTriplesPerPatternInstance
            enif_get_bool(env, argv[2]),  // bern
            enif_get_bool(env, argv[3]),  // crossSampling
            enif_get_long_(env, argv[4]),  // nWorkers
            enif_get_bool(env, argv[5])   // verbose
        );
    } catch (invalidArgument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}

extern ERL_NIF_TERM
sample_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try{
        INT batchSize = enif_get_long_(env, argv[0]); 

        INT entityNegativeRate = enif_get_long_(env, argv[1]); 
        INT relationNegativeRate = enif_get_long_(env, argv[2]); 

        bool headBatchFlag = enif_get_bool(env, argv[3]);
        bool verbose = enif_get_bool(env, argv[4]);

        sampling::batch::Triple* tripleBatch = sample(batchSize, entityNegativeRate, relationNegativeRate, headBatchFlag, verbose);

        ERL_NIF_TERM encoded = encodeTripleBatch(env, tripleBatch);

        return completed_with_success(env, encoded);
    } catch (invalidArgument& e) {
        return completed_with_error(env, e.what());
    }
}

ERL_NIF_TERM encodeTripleBatch(ErlNifEnv* env, sampling::batch::Triple* tripleBatch, bool encodeLabels) {

    int nElements = encodeLabels ? 4 : 3;

    INT batchSize = tripleBatch->length;

    // cout << "Batch size = " << batchSize << endl;

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batchSize]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batchSize]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batchSize]();
    ERL_NIF_TERM* batch_y;

    if (encodeLabels) {
        batch_y = new ERL_NIF_TERM[batchSize]();
        enif_encode_array_of_float(env, tripleBatch->labels, batch_y, batchSize);
    }

    enif_encode_array_of_long(env, tripleBatch->head, batch_h, batchSize);
    enif_encode_array_of_long(env, tripleBatch->tail, batch_t, batchSize);
    enif_encode_array_of_long(env, tripleBatch->relation, batch_r, batchSize);

    delete tripleBatch;


    ERL_NIF_TERM* batch = new ERL_NIF_TERM[nElements]();

    batch[0] = enif_make_list_from_array(env, batch_h, batchSize);
    batch[1] = enif_make_list_from_array(env, batch_t, batchSize);
    batch[2] = enif_make_list_from_array(env, batch_r, batchSize);

    if (encodeLabels) {
        batch[3] = enif_make_list_from_array(env, batch_y, batchSize);
        delete [] batch_y;
    }

    delete [] batch_h;
    delete [] batch_t;
    delete [] batch_r;
    
    ERL_NIF_TERM result = enif_make_list_from_array(env, batch, nElements);

    delete [] batch;

    return result;
}
