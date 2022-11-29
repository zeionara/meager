#ifndef PATTERNS_MAIN_H
#define PATTERNS_MAIN_H

#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "../triple/main.h"

using namespace std;

enum Pattern {
    none = 0,
    symmetric = 1,
    inverse = 2
};

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
            result += to_string(triple.h) + "|" + to_string(triple.r) + "|" + to_string(triple.t) + "\n";
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

struct PatternDescription {
    string label;
    Pattern id;
    int nTriplesPerInstance;
    vector<PatternInstance>** instanceSets;

    void initInstanceLists() {
        instanceSets = (vector<PatternInstance>**)malloc(sizeof(vector<PatternInstance>*) * (nTriplesPerInstance + 1));

        for (int i = 0; i <= nTriplesPerInstance; i++) {
            instanceSets[i] = new vector<PatternInstance>;
        }
    }

    void push(PatternInstance patternInstance) {
        for (int j = 0; j <= nTriplesPerInstance; j++) {
            if (j <= patternInstance.observedTripleIndices.size()) {
               instanceSets[j]->push_back(patternInstance); 
            }
        }
    }

    void tryPush(PatternInstance patternInstance, unordered_set<string> seenInstances, bool dropDuplicates = true) {
        if (dropDuplicates) {
            string direct_pattern_instance_concise_description = patternInstance.getConciseDescription();

            if (seenInstances.find(direct_pattern_instance_concise_description) == seenInstances.end()) {
                push(patternInstance);
                seenInstances.insert(direct_pattern_instance_concise_description);
            }
            // } else {
            //     cout << direct_pattern_instance_concise_description << endl;
            // }
        } else {
            push(patternInstance);
        }
    }

    void printSummary(INT nInstancesTarget = -1) {
        if (nInstancesTarget > -1) {
            cout << "Should collect " << nInstancesTarget << " " << label << " pattern instances" << endl;
        }

        for (int i = 0; i <= nTriplesPerInstance; i++) {
            cout << "Collected " << instanceSets[i]->size() << " " << label << " pattern instances in which there are " << i << " or more observed patterns" << endl;
        }

        cout << endl;
    }

};

Pattern decodePatternName(string name);

#endif
