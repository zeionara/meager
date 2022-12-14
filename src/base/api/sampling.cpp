#include "../sampling/Pattern.h"
// #include "../triple/pattern/main.h"

#include "corpus.h"

namespace meager::main::api::sampling {

    INT validateNobservedTriplesPerPatternInstance(triple::pattern::Pattern pattern, INT nObservedTriplesPerPatternInstance);

    main::sampling::Pattern<INT, INT>* sampler;

    void init(triple::pattern::Pattern pattern, INT nObservedTriplesPerPatternInstance, bool bern, bool crossSampling, INT nWorkers, bool verbose) {
        sampler = new main::sampling::Pattern<INT, INT>(pattern, validateNobservedTriplesPerPatternInstance(pattern, nObservedTriplesPerPatternInstance), bern, crossSampling, nWorkers, verbose);
    }

    main::sampling::batch::Triple* sample(INT batchSize, INT entityNegativeRate, INT relationNegativeRate, bool headBatchFlag, bool verbose) {
        return sampler->sample(corpus::corpus, batchSize, entityNegativeRate, relationNegativeRate, headBatchFlag, verbose);
    }

    INT validateNobservedTriplesPerPatternInstance(triple::pattern::Pattern pattern, INT nObservedTriplesPerPatternInstance) {
        triple::pattern::description::Description patternDescription = corpus::corpus->train->patterns->get(pattern);

        INT nTriplesPerPatternInstance = patternDescription.nTriplesPerInstance;

        if (nObservedTriplesPerPatternInstance > nTriplesPerPatternInstance) {
            cout << 
                "Requested number of observed triples exceeds number of triples per instance of the selected pattern (" <<
                nObservedTriplesPerPatternInstance << " > " << nTriplesPerPatternInstance <<
                "); using the highest available value (" << nTriplesPerPatternInstance << ")" <<
            endl;
            return nTriplesPerPatternInstance;
        }

        if (nObservedTriplesPerPatternInstance < 0) {
            cout << "Requested number of observed triples is negative; using the lowest available value (0)" << endl;
            return 0;
        }

        return nObservedTriplesPerPatternInstance;
    }

}