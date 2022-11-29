#ifndef PATTERNS_INVERSE_PATTERN_INSTANCE_H
#define PATTERNS_INVERSE_PATTERN_INSTANCE_H

#include <cstdlib>

#include "../main.h"
#include "../../triple/TripleIndex.h"

const string inversePatternName = "inverse";

struct InversePatternInstance: PatternInstance {

    InversePatternInstance(Triple forward, Triple backward, TripleIndex* index, bool isForwardObserved = true) {
        triples = {forward, backward};
        if (isForwardObserved) {
            observedTripleIndices = {0};

            if (index->contains(backward)) {
                observedTripleIndices.insert(1);
            }
        } else {
            observedTripleIndices = {1};

            if (index->contains(forward)) {
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
