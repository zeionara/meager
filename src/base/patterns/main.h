#ifndef PATTERNS_MAIN_H
#define PATTERNS_MAIN_H

#include <vector>
#include <unordered_set>

// #include "../triple/main.h"
#include "../triple/Triple.h"

using namespace std;
using namespace meager::main;

enum Pattern {
    none = 0,
    symmetric = 1,
    inverse = 2
};

struct PatternInstance {
    vector<triple::Triple> triples;
    unordered_set<int> observedTripleIndices;

    string getObservanceMark(int i) {
        return observedTripleIndices.find(i) == observedTripleIndices.end() ? " - " : " + ";
    }

    string getConciseDescription() {
        string result = "";
        // int i = 0;
        for (triple::Triple triple: triples) {
            result += to_string(triple.h) + "|" + to_string(triple.r) + "|" + to_string(triple.t) + "\n";
        }
        return result;
    }

    void print(string label) {
        cout << "--- " << label << " triples pattern instance"  << endl;
        int i = 0;
        for (triple::Triple triple: triples) {
            cout << "| head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(i) << endl;
        }
        cout << "---" << endl;
    }
};

Pattern decodePatternName(string name);

#endif
