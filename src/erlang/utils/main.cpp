#include "erl_nif.h"

#include "../../base/sampling/batch/Triple.h"

#include "nif/encode.h"

namespace meager::erlang::utils::encode {

    ERL_NIF_TERM tripleBatch(ErlNifEnv* env, main::sampling::batch::Triple* tripleBatch, bool encodeLabels) {

        int nElements = encodeLabels ? 4 : 3;

        INT batchSize = tripleBatch->length;

        // cout << "Batch size = " << batchSize << endl;

        ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batchSize]();
        ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batchSize]();
        ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batchSize]();
        ERL_NIF_TERM* batch_y;

        if (encodeLabels) {
            batch_y = new ERL_NIF_TERM[batchSize]();
            nif::encode::list(env, tripleBatch->labels, batch_y, batchSize);
        }

        nif::encode::list(env, tripleBatch->head, batch_h, batchSize);
        nif::encode::list(env, tripleBatch->tail, batch_t, batchSize);
        nif::encode::list(env, tripleBatch->relation, batch_r, batchSize);

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
