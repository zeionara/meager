#ifndef PATTERNS_NONE_PATTERN_INSTANCE_H
#define PATTERNS_NONE_PATTERN_INSTANCE_H

#include <cstdlib>

#include "../main.h"

const string nonePatternName = "none";

struct NonePatternInstance: PatternInstance {

    NonePatternInstance(Triple triple) {
        triples = {triple};
        observedTripleIndices = {0};
    }

    void print(string label = nonePatternName) {
        cout << "--- " << label << " triples pattern instance"  << endl;
        Triple triple = triples[0];
        cout << "| head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(0) << endl;
        cout << "---" << endl;
    }

};

#endif
