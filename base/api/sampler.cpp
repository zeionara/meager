#include "../samplers/PatternSampler.h"
#include "../patterns/main.h"

PatternSampler<INT>* sampler;

void initSampler(Pattern pattern, INT nObservedTriplesPerPatternInstance, bool bern, bool crossSampling, INT nWorkers, bool verbose) {
    sampler = new PatternSampler<INT>(pattern, nObservedTriplesPerPatternInstance, bern, crossSampling, nWorkers);
}
