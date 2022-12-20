#include "../base/api/sampling.h"
#include "utils/main.h"

extern "C" void meager__python__api__sampling__init(char* pattern, long nObservedTriplesPerPatternInstance, bool bern, bool crossSampling, long nWorkers, bool verbose) {
    meager::main::api::sampling::init(
        triple::pattern::decodeName(pattern), nObservedTriplesPerPatternInstance, bern, crossSampling, nWorkers, verbose
    );
}

// meager::main::sampling::batch::Triple* lastSampledBatch = NULL;

extern "C" void** meager__python__api__sampling__sample(
    long size, long entityNegativeRate, long relationNegativeRate, bool headBatch, bool verbose
) {
    auto batch = meager::main::api::sampling::sample(
        size, entityNegativeRate, relationNegativeRate, headBatch, verbose
    );

    // lastSampledBatch = batch;

    return meager::python::utils::encode::tripleBatch(batch);
}
