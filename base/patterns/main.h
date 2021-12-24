#ifndef PATTERNS_MAIN_H
#define PATTERNS_MAIN_H

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "../Triple.h"

using namespace std;


void* getPatternBatch(void* con);

enum Pattern {
    none = 0,
    symmetric = 1
};

struct PatternDescription {
    Pattern id;
    int nTriplesPerInstance;

    PatternDescription(Pattern id_ = none, int nTriplesPerInstance_ = 0) {
        id = id_;
        nTriplesPerInstance = nTriplesPerInstance_; 
    }
};
 
extern
unordered_map<string, PatternDescription> patternDescriptions;

struct PatternInstance {
    vector<Triple> triples;
    unordered_set<int> observedTripleIndices;

    string getObservanceMark(int i) {
        return observedTripleIndices.find(i) == observedTripleIndices.end() ? " - " : " + ";
    }

    string getConciseDescription() {
        string result = "";
        int i = 0;
        for (Triple triple: triples) {
            result += to_string(triple.h) + "|" + to_string(triple.r) + "|" + to_string(triple.h) + "\n";
        }
        return result;
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

