#ifndef SAMPLER_H
#define SAMPLER_H

#include <string>

#include "../triple/main.h"
#include "../patterns/main.h"
#include "../storage/Corpus.h"

#include "TripleBatch.h"

using namespace std;

template <typename T>
struct Sampler {
    virtual TripleBatch* sample(T* corpus, INT batchSize, INT entityNegativeRate, INT relationNegativeRate, bool headBatchFlag, bool verbose = false) = 0;
};

#endif
