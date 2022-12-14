#ifndef API_SAMPLING_H
#define API_SAMPLING_H

// #include "../triple/pattern/main.h"
#include "../sampling/Pattern.h"

namespace meager::main::api::sampling {

    extern
    main::sampling::Pattern<INT, INT>* sampler;

    void init(triple::pattern::Pattern pattern, INT nObservedTriplesPerPatternInstance, bool bern = false, bool crossSampling = false, INT nWorkers = 8, bool verbose = false);
    main::sampling::batch::Triple* sample(INT batchSize, INT entityNegativeRate, INT relationNegativeRate, bool headBatchFlag, bool verbose);

}

#endif
