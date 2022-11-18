#ifndef SAMPLER_H
#define SAMPLER_H

#include <string>

#include "../triple/main.h"
#include "../patterns/main.h"

#include "TripleBatch.h"

using namespace std;

struct Sampler {
    virtual TripleBatch* sample(INT batchSize, INT entityNegativeRate, INT relationNegativeRate, INT headBatchFlag) = 0;
};

#endif
