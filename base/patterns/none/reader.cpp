#include <fstream>
#include <vector>
#include <unordered_set>

#include "main.h"
#include "../../Reader.h"

using namespace std;

const int nTriplesPerPattern = 1; // patternDescriptions[nonePatternName].nTriplesPerInstance;

vector<PatternInstance>** noneTriplePatternInstances = (vector<PatternInstance>**)malloc(sizeof(vector<PatternInstance>*) * (nTriplesPerPattern + 1));

void separateNoneTriples(bool verbose = false, bool drop_duplicates = true) {
    if (verbose) {
		cout << "Writing triples as none pattern instances..." << endl;
    }

    unordered_set<string> seenInstances;

    for (int i = 0; i <= nTriplesPerPattern; i++) {
        noneTriplePatternInstances[i] = new vector<PatternInstance>;
    }

	for (INT i = 0; i < trainTotal; i++) { // Reading train samples
        NonePatternInstance patternInstance = NonePatternInstance(
                trainList[i]
        );

        if (drop_duplicates) {
            string direct_pattern_instance_concise_description = patternInstance.getConciseDescription();
            if (seenInstances.find(direct_pattern_instance_concise_description) == seenInstances.end()) {
                for (int j = 0; j <= nTriplesPerPattern; j++) {
                    if (j <= patternInstance.observedTripleIndices.size()) {
                       noneTriplePatternInstances[j]->push_back(patternInstance); 
                    }
                }
                seenInstances.insert(direct_pattern_instance_concise_description);
            }
        } else {
                for (int j = 0; j <= nTriplesPerPattern; j++) {
                    if (j <= patternInstance.observedTripleIndices.size()) {
                       noneTriplePatternInstances[j]->push_back(patternInstance); 
                    }
                }
        }
	}

    if (verbose) {
        for (int i = 0; i <= nTriplesPerPattern; i++) {
            cout << "Collected " << noneTriplePatternInstances[i]->size() << " none pattern instances in which there are " << i << " or more observed patterns" << endl;
        }

        std::cout << std::endl;
        // std::cout << std::endl << std::endl;
        
        // for (PatternInstance patternInstance: **noneTriplePatternInstances) {
        //     ((NonePatternInstance*)&patternInstance)->print();
        // }
    }

    // patternDescriptions[nonePatternName].instanceSets = noneTriplePatternInstances;
    patternDescriptions[nonePatternName] = PatternDescription(none, nTriplesPerPattern, noneTriplePatternInstances);
}

