#ifndef SYMMETRIC_PATTERN_MAIN
#define SYMMETRIC_PATTERN_MAIN

#include <cstdlib>

#include "../main.h"

void* getSymmetricBatch(void* con);

struct SymmetricPatternInstance: PatternInstance {
    // void print() {
    //     PatternInstance::print("symmetric");
    // }
    
    SymmetricPatternInstance(Triple forward, Triple backward, bool isForwardObserved = true) {
        triples = {forward, backward};
        observedTripleIndices = {isForwardObserved ? 0 : 1};
    }

    void print(string label = "symmetric") {
        cout << "--- " << label << " triples pattern instance"  << endl;
        // for (Triple triple: triples) {
        Triple triple = triples[0];
        cout << "| forward : head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(0) << endl;
        triple = triples[1];
        cout << "| backward: head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(1) << endl;
        // }
        cout << "---" << endl;
    }
};

#endif

