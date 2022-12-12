#ifndef SAMPLING_SAMPLER_H
#define SAMPLING_SAMPLER_H

// #include <string>

// #include "../triple/main.h"
// #include "../triple/pattern/main.h"
// #include "../storage/Corpus.h"

#include "batch/Triple.h"

using namespace std;

namespace meager::main::sampling {

    template <typename T>
    struct Sampler {
        virtual batch::Triple* sample(T* corpus, INT batchSize, INT entityNegativeRate, INT relationNegativeRate, bool headBatchFlag, bool verbose = false) = 0;
    };

}

#endif
