#include "erl_nif.h"

#include "utils/nif/decode.h"
#include "utils/nif/complete.h"

#include "../base/api/sampling.h"

// using namespace meager;

namespace meager::erlang::api::sampling {

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

            ERL_NIF_TERM encoded = utils::encode::tripleBatch(env, tripleBatch);

            return utils::nif::complete::success(env, encoded);
        } catch (invalidArgument& e) {
            return utils::nif::complete::error(env, e.what());
        }
    }

}
