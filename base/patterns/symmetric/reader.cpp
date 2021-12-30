#include <fstream>
#include <vector>
#include <unordered_set>

#include "main.h"
#include "../../Reader.h"

using namespace std;

const int nTriplesPerPattern = 2;

// vector<PatternInstance> symmetricTriples;
vector<PatternInstance>** symmetricTriplePatternInstances = (vector<PatternInstance>**)malloc(sizeof(vector<PatternInstance>*) * (nTriplesPerPattern + 1));

void separateSymmetricTriples(bool verbose = false, bool drop_duplicates = true) {
    if (verbose) {
		cout << "Separating symmetric triples..." << endl;
    }

    ifstream in_file(inPath + "patterns/symmetric.txt");

    int relation;
    unordered_set<int> symmetricRelations;

    while (in_file >> relation) {
        if (verbose) {
            printf("Read relation %d.\n", relation);
        }

        symmetricRelations.insert(relation);
    }
    
    if (verbose) {
        printf("Number of symmetric relations = %d.\n", (int)symmetricRelations.size());
    }

    in_file.close();

    if (verbose) {
        cout << "Symmetric triples" << endl;
    }

    unordered_set<string> seenInstances;

    for (int i = 0; i <= nTriplesPerPattern; i++) {
        symmetricTriplePatternInstances[i] = new vector<PatternInstance>;
    }

    auto pushPatternInstance = [drop_duplicates, &seenInstances](PatternInstance patternInstance) {
        if (drop_duplicates) {
            string direct_pattern_instance_concise_description = patternInstance.getConciseDescription();
            if (seenInstances.find(direct_pattern_instance_concise_description) == seenInstances.end()) {
                // symmetricTriples.push_back(
                //         patternInstance
                // );
                for (int j = 0; j <= nTriplesPerPattern; j++) {
                    if (j <= patternInstance.observedTripleIndices.size()) {
                       symmetricTriplePatternInstances[j]->push_back(patternInstance); 
                    }
                }
                seenInstances.insert(direct_pattern_instance_concise_description);
            }
        } else {
                // symmetricTriples.push_back(
                //         patternInstance
                // );
                for (int j = 0; j <= nTriplesPerPattern; j++) {
                    if (j <= patternInstance.observedTripleIndices.size()) {
                       symmetricTriplePatternInstances[j]->push_back(patternInstance); 
                    }
                }
        }
    };

	for (INT i = 0; i < trainTotal; i++) { // Reading train samples
        Triple triple = trainList[i];

        if (symmetricRelations.find(triple.r) != symmetricRelations.end()) {
            auto direct_pattern_instance = SymmetricPatternInstance(
                    triple,
                    Triple(triple.t, triple.r, triple.h)
            );
            pushPatternInstance(direct_pattern_instance);

            auto inverse_pattern_instance = SymmetricPatternInstance(
                Triple(triple.t, triple.r, triple.h),
                triple,
                false
            );
            pushPatternInstance(inverse_pattern_instance);
        }
	}

    if (verbose) {
        for (int i = 0; i <= nTriplesPerPattern; i++) {
            cout << "Collected " << symmetricTriplePatternInstances[i]->size() << " symmetric pattern instances in which there are " << i << " or more observed patterns" << endl;
        }

        std::cout << std::endl;
        // std::cout << std::endl << std::endl;
        // 
        // for (PatternInstance patternInstance: **symmetricTriplePatternInstances) {
        //     ((SymmetricPatternInstance*)&patternInstance)->print();
        // }
    }

    // patternDescriptions[symmetricPatternName].instanceSets = symmetricTriplePatternInstances;
    patternDescriptions[symmetricPatternName] = PatternDescription(symmetric, nTriplesPerPattern, symmetricTriplePatternInstances);
}

