#include <fstream>
#include <vector>
#include <unordered_set>

#include "main.h"
#include "../../Reader.h"

using namespace std;

const int nTriplesPerPattern = 2;

vector<PatternInstance> symmetricTriples;
vector<PatternInstance> symmetricTriplePatternInstances[nTriplesPerPattern + 1];
// int nSymmetricTriples = 0;

void separateSymmetricTriples(bool verbose = false) {
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

	for (INT i = 0; i < trainTotal; i++) { // Reading train samples
        Triple triple = trainList[i];

        if (symmetricRelations.find(triple.r) != symmetricRelations.end()) {
            // SymmetricPatternInstance* patternInstance = new SymmetricPatternInstance(triple, Triple(triple.t, triple.r, triple.h));
            // (new SymmetricPatternInstance(Triple(triple.t, triple.r, triple.h), triple, false))->print();

            // patternInstance->print();

            // symmetricTriples.push_back(patternInstance->triples);

            auto direct_pattern_instance = SymmetricPatternInstance(
                    triple,
                    Triple(triple.t, triple.r, triple.h)
            );
            symmetricTriples.push_back(
                    direct_pattern_instance
            );

            auto inverse_pattern_instance = SymmetricPatternInstance(
                Triple(triple.t, triple.r, triple.h),
                triple,
                false
            );
            symmetricTriples.push_back(
                    inverse_pattern_instance
            );

            for (int j = 0; j <= nTriplesPerPattern; j++) {
                if (j <= direct_pattern_instance.observedTripleIndices.size()) {
                   symmetricTriplePatternInstances[j].push_back(direct_pattern_instance); 
                }
                if (j <= inverse_pattern_instance.observedTripleIndices.size()) {
                   symmetricTriplePatternInstances[j].push_back(inverse_pattern_instance); 
                }
            }

            // nSymmetricTriples++;
        }
	}

    for (int i = 0; i <= nTriplesPerPattern; i++) {
        cout << "Collected " << symmetricTriplePatternInstances[i].size() << " pattern instances in which there are " << i << " or more observed patterns" << endl;
    }

    std::cout << std::endl << std::endl;
    
    for (PatternInstance patternInstance: symmetricTriples) {
        ((SymmetricPatternInstance*)&patternInstance)->print();
        // for (Triple triple: pair) {
        //     triple.print();
        // }
    }
}

