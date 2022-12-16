#include "../base/api/sampling.h"
#include "utils/main.h"

extern "C" void meager__python__api__sampling__init(char* pattern, long nObservedTriplesPerPatternInstance, bool bern, bool crossSampling, long nWorkers, bool verbose) {
    meager::main::api::sampling::init(
        triple::pattern::decodeName(pattern), nObservedTriplesPerPatternInstance, bern, crossSampling, nWorkers, verbose
    );
}

extern "C" void** meager__python__api__sampling__sample(
    long size, long entityNegativeRate, long relationNegativeRate, bool headBatch, bool verbose,
    long* batch_h, long* batch_t, long* batch_r, float* batch_y
) {
    auto batch = meager::main::api::sampling::sample(
        size, entityNegativeRate, relationNegativeRate, headBatch, verbose
    );

    return meager::python::utils::encode::tripleBatch(batch);

}

