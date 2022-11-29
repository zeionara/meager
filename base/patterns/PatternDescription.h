#ifndef PATTERNS_PATTERN_DESCRIPTION_H
#define PATTERNS_PATTERN_DESCRIPTION_H

#include "main.h"

using namespace std;

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

#endif
