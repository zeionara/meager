#include "erl_nif.h"
#include "utils.h"

#include "../base/api/sampling.h"

// using namespace meager;

namespace meager::erlang::api::sampling {

    ERL_NIF_TERM encodeTripleBatch(ErlNifEnv* env, main::sampling::batch::Triple* tripleBatch, bool verbose = false);

    extern ERL_NIF_TERM
    init(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try {
            triple::pattern::Pattern pattern = triple::pattern::decodeName(utils::nif::decode::atom(env, argv[0]));

            main::api::sampling::init(
                pattern,  // pattern
                utils::nif::decode::longInteger(env, argv[1]),  // nObservedTriplesPerPatternInstance
                utils::nif::decode::boolean(env, argv[2]),  // bern
                utils::nif::decode::boolean(env, argv[3]),  // crossSampling
                utils::nif::decode::longInteger(env, argv[4]),  // nWorkers
                utils::nif::decode::boolean(env, argv[5])   // verbose
            );
        } catch (invalidArgument& e) {
            return utils::nif::complete::error(env, e.what());
        }

        return utils::nif::complete::success(env);
    }

    extern ERL_NIF_TERM
    sample(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
        try{
            INT batchSize = utils::nif::decode::longInteger(env, argv[0]); 

            INT entityNegativeRate = utils::nif::decode::longInteger(env, argv[1]); 
            INT relationNegativeRate = utils::nif::decode::longInteger(env, argv[2]); 

            bool headBatchFlag = utils::nif::decode::boolean(env, argv[3]);
            bool verbose = utils::nif::decode::boolean(env, argv[4]);

            main::sampling::batch::Triple* tripleBatch = main::api::sampling::sample(batchSize, entityNegativeRate, relationNegativeRate, headBatchFlag, verbose);

            ERL_NIF_TERM encoded = encodeTripleBatch(env, tripleBatch);

            return utils::nif::complete::success(env, encoded);
        } catch (invalidArgument& e) {
            return utils::nif::complete::error(env, e.what());
        }
    }

    ERL_NIF_TERM encodeTripleBatch(ErlNifEnv* env, main::sampling::batch::Triple* tripleBatch, bool encodeLabels) {

        int nElements = encodeLabels ? 4 : 3;

        INT batchSize = tripleBatch->length;

        // cout << "Batch size = " << batchSize << endl;

        ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batchSize]();
        ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batchSize]();
        ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batchSize]();
        ERL_NIF_TERM* batch_y;

        if (encodeLabels) {
            batch_y = new ERL_NIF_TERM[batchSize]();
            utils::nif::encode::list(env, tripleBatch->labels, batch_y, batchSize);
        }

        utils::nif::encode::list(env, tripleBatch->head, batch_h, batchSize);
        utils::nif::encode::list(env, tripleBatch->tail, batch_t, batchSize);
        utils::nif::encode::list(env, tripleBatch->relation, batch_r, batchSize);

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

}
