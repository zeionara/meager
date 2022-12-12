#ifndef API_SAMPLER_H
#define API_SAMPLER_H

#include "../triple/pattern/main.h"
#include "../sampling/Pattern.h"

extern
sampling::Pattern<INT, INT>* sampler;

void initSampler(triple::pattern::Pattern pattern, INT nObservedTriplesPerPatternInstance, bool bern = false, bool crossSampling = false, INT nWorkers = 8, bool verbose = false);
sampling::batch::Triple* sample(INT batchSize, INT entityNegativeRate, INT relationNegativeRate, bool headBatchFlag, bool verbose);

#endif
