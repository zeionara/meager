#include <fstream>
#include <vector>
#include <unordered_set>

#include "main.h"
#include "../../Reader.h"

using namespace std;

const int nTriplesPerPattern = 2;

// vector<PatternInstance> symmetricTriples;
vector<PatternInstance>** symmetricTriplePatternInstances = (vector<PatternInstance>**)malloc(sizeof(vector<PatternInstance>*) * (nTriplesPerPattern + 1));

void separateSymmetricTriples(string path, Triple* triples, INT nTriples, bool verbose, bool drop_duplicates) {
    if (verbose) {
		cout << "Separating symmetric triples..." << endl;
    }

    ifstream in_file(path + "patterns/symmetric.txt");

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
            // cout << "pushing found triple" << endl;
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

	for (INT i = 0; i < nTriples; i++) { // Reading train samples
        Triple triple = triples[i];

        if (symmetricRelations.find(triple.r) != symmetricRelations.end()) {
            // cout << triple.r << " is a symmetric relation" << endl;
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
        // else {
        //     cout << triple.r << " is not a symmetric relation" << endl;
        // }
        // cout << "Collected " << symmetricTriplePatternInstances[1]->size() << " symmetric pattern instances in which there are " << 1 << " or more observed patterns" << endl;
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
    patternDescriptions[symmetric] = PatternDescription(symmetric, nTriplesPerPattern, symmetricTriplePatternInstances);
}

