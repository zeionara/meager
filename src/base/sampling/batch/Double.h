#ifndef SAMPLING_BATCH_DOUBLE_H
#define SAMPLING_BATCH_DOUBLE_H

#include "Triple.h"

namespace meager::main::sampling::batch {

    struct Double {
        Triple* positive;
        Triple* negative;

        Double(Triple* positive, Triple* negative) {
            this->positive = positive;
            this->negative = negative;
        };
    };

}

#endif
