#ifndef PATTERNS_NONE_PATTERN_DESCRIPTION_H
#define PATTERNS_NONE_PATTERN_DESCRIPTION_H

#include "main.h"

#include "../../triple/TripleIndex.h"
#include "../../triple/list/TripleList.h"

struct NonePatternDescription: PatternDescription {
    // Pattern id;
    // int nTriplesPerInstance;
    // vector<PatternInstance>** instanceSets;

    NonePatternDescription(TripleList* triples, bool verbose = false, bool dropDuplicates = true) {
        INT nTriplesPerInstance_ = 1;
        // cout << sizeof(vector<PatternInstance>*) * (nTriplesPerInstance_ + 1) << endl;
        vector<PatternInstance>** instances = (vector<PatternInstance>**)malloc(sizeof(vector<PatternInstance>*) * (nTriplesPerInstance_ + 1));

        unordered_set<string> seenInstances;

        for (int i = 0; i <= nTriplesPerInstance_; i++) {
            instances[i] = new vector<PatternInstance>;
        }

        // cout << "none triples handling checkpoint #1" << endl;

        for (INT i = 0; i < triples->length; i++) { // Reading train samples
            // cout << "none triples handling checkpoint #2" << endl;

            NonePatternInstance patternInstance = NonePatternInstance(
                triples->items[i]
            );

            // cout << "none triples handling checkpoint #3" << endl;

            if (dropDuplicates) {
                string direct_pattern_instance_concise_description = patternInstance.getConciseDescription();
                if (seenInstances.find(direct_pattern_instance_concise_description) == seenInstances.end()) {
                    for (int j = 0; j <= nTriplesPerInstance_; j++) {
                        if (j <= patternInstance.observedTripleIndices.size()) {
                           instances[j]->push_back(patternInstance); 
                        }
                    }
                    seenInstances.insert(direct_pattern_instance_concise_description);
                }
                // } else {
                //     cout << direct_pattern_instance_concise_description << endl;
                // }
            } else {
                for (int j = 0; j <= nTriplesPerInstance_; j++) {
                    if (j <= patternInstance.observedTripleIndices.size()) {
                       instances[j]->push_back(patternInstance); 
                    }
                }
            }
        }

        cout << "Should collect " << triples->length << " none pattern instances" << endl;

        if (verbose) {
            for (int i = 0; i <= nTriplesPerInstance_; i++) {
                cout << "Collected " << instances[i]->size() << " none pattern instances in which there are " << i << " or more observed patterns" << endl;
            }

            std::cout << std::endl;
            // std::cout << std::endl << std::endl;
            
            // for (PatternInstance patternInstance: **noneTriplePatternInstances) {
            //     ((NonePatternInstance*)&patternInstance)->print();
            // }
        }

        // patternDescriptions[nonePatternName].instanceSets = noneTriplePatternInstances;

        id = none;
        cout << "Setting n triples per instance" << endl;
        nTriplesPerInstance = 1; 
        cout << "Set n triples per instance = " << nTriplesPerInstance << endl;
        instanceSets = instances;
    }
};

#endif
