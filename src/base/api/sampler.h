#ifndef API_SAMPLER_H
#define API_SAMPLER_H

#include "../triple/pattern/main.h"
#include "../samplers/PatternSampler.h"

extern
PatternSampler<INT>* sampler;

void initSampler(triple::pattern::Pattern pattern, INT nObservedTriplesPerPatternInstance, bool bern = false, bool crossSampling = false, INT nWorkers = 8, bool verbose = false);
TripleBatch* sample(INT batchSize, INT entityNegativeRate, INT relationNegativeRate, bool headBatchFlag, bool verbose);

#endif
