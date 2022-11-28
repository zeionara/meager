#ifndef NONE_PATTERN_MAIN
#define NONE_PATTERN_MAIN

#include <cstdlib>

#include "../main.h"

// #include "../../Reader.h"

extern
const string nonePatternName;

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

