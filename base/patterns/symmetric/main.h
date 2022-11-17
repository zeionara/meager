#ifndef SYMMETRIC_PATTERN_MAIN
#define SYMMETRIC_PATTERN_MAIN

#include <cstdlib>

#include "../main.h"
#include "../../triple/list/main.h"

#include "../../Reader.h"

extern
const string symmetricPatternName;

struct SymmetricPatternInstance: PatternInstance {
    SymmetricPatternInstance(Triple forward, Triple backward, bool isForwardObserved = true) {
        triples = {forward, backward};
        if (isForwardObserved) {
            observedTripleIndices = {0};

            if (trainList->index->contains(backward)) {
                observedTripleIndices.insert(1);
            }
        } else {
            observedTripleIndices = {1};

            if (trainList->index->contains(forward)) {
                observedTripleIndices.insert(0);
            }
        }
    }

    void print(string label = symmetricPatternName) {
        cout << "--- " << label << " triples pattern instance"  << endl;
        Triple triple = triples[0];
        cout << "| forward : head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(0) << endl;
        triple = triples[1];
        cout << "| backward: head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(1) << endl;
        cout << "---" << endl;
    }
};

#endif

