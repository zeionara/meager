#ifndef INVERSE_PATTERN_MAIN
#define INVERSE_PATTERN_MAIN

#include <cstdlib>

#include "../main.h"

#include "../../Reader.h"

extern
const string inversePatternName;

struct InversePatternInstance: PatternInstance {
    InversePatternInstance(Triple forward, Triple backward, bool isForwardObserved = true) {
        triples = {forward, backward};
        if (isForwardObserved) {
            observedTripleIndices = {0};

            if (trainTripleIndex->contains(backward)) {
                observedTripleIndices.insert(1);
            }
        } else {
            observedTripleIndices = {1};

            if (trainTripleIndex->contains(forward)) {
                observedTripleIndices.insert(0);
            }
        }
    }

    void print(string label = inversePatternName) {
        cout << "--- " << label << " triples pattern instance"  << endl;
        Triple triple = triples[0];
        cout << "| forward : head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(0) << endl;
        triple = triples[1];
        cout << "| backward: head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(1) << endl;
        cout << "---" << endl;
    }
};

#endif

