#ifndef PATTERNS_MAIN
#define PATTERNS_MAIN

#include <vector>
#include <unordered_set>

#include "../Triple.h"

using namespace std;

struct PatternInstance {
    vector<Triple> triples;
    unordered_set<int> observedTripleIndices;

    string getObservanceMark(int i) {
        return observedTripleIndices.find(i) == observedTripleIndices.end() ? " - " : " + ";
    }

    void print(string label) {
        cout << "--- " << label << " triples pattern instance"  << endl;
        int i = 0;
        for (Triple triple: triples) {
            cout << "| head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(i) << endl;
        }
        cout << "---" << endl;
    }
};

#endif

