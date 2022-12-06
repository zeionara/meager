#ifndef SAMPLERS_DOUBLE_BATCH_H
#define SAMPLERS_DOUBLE_BATCH_H

#include "../triple/main.h"
#include "TripleBatch.h"

struct DoubleBatch {
    TripleBatch* positive;
    TripleBatch* negative;

    DoubleBatch(TripleBatch* positive, TripleBatch* negative) {
        this->positive = positive;
        this->negative = negative;
    };
};

#endif
