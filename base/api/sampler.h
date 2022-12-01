#ifndef API_SAMPLER_H
#define API_SAMPLER_H

#include "../patterns/main.h"
#include "../samplers/PatternSampler.h"

extern
PatternSampler<INT>* sampler;

void initSampler(Pattern pattern, INT nObservedTriplesPerPatternInstance, bool bern = false, bool crossSampling = false, INT nWorkers = 8, bool verbose = false);

#endif
