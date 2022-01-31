#include <fstream>
#include <vector>
#include <unordered_set>

#include "main.h"
#include "../../Reader.h"

using namespace std;

const int nTriplesPerPattern = 2;

// vector<PatternInstance> symmetricTriples;
vector<PatternInstance>** inverseTriplePatternInstances = (vector<PatternInstance>**)malloc(sizeof(vector<PatternInstance>*) * (nTriplesPerPattern + 1));

void separateInverseTriples(bool verbose = false, bool drop_duplicates = true, bool enable_filters = false) {
    if (verbose) {
		cout << "Separating inverse triples..." << endl;
    }

    ifstream in_file(inPath + "patterns/inverse.txt");

    int forwardRelation, backwardRelation;
    // unordered_set<int> inverseForwardRelations;
    // unordered_set<int> inverseBackwardRelations;

    unordered_map<int, int> inverseForwardRelationToBackward;
    unordered_map<int, int> inverseBackwardRelationToForward;

    // cout << forwardRelation << endl;
    // cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

    while (in_file >> forwardRelation >> backwardRelation) {
        if (verbose) {
            printf("Read relations %d and %d.\n", forwardRelation, backwardRelation);
        }

        // inverseForwardRelations.insert(forwardRelation);
        // inverseBackwardRelations.insert(backwardRelation);

        inverseForwardRelationToBackward[forwardRelation] = backwardRelation;
        inverseBackwardRelationToForward[backwardRelation] = forwardRelation;
    }
    
    if (verbose) {
        printf("Number of inverse relations = %d.\n", (int)inverseForwardRelationToBackward.size());
    }

    in_file.close();

    if (verbose) {
        cout << "Inverse triples" << endl;
    }

    unordered_set<string> seenInstances;

    for (int i = 0; i <= nTriplesPerPattern; i++) {
        inverseTriplePatternInstances[i] = new vector<PatternInstance>;
    }

    auto pushPatternInstance = [drop_duplicates, &seenInstances](PatternInstance patternInstance) {
        if (drop_duplicates) {
            string direct_pattern_instance_concise_description = patternInstance.getConciseDescription();
            if (seenInstances.find(direct_pattern_instance_concise_description) == seenInstances.end()) {
                for (int j = 0; j <= nTriplesPerPattern; j++) {
                    if (j <= patternInstance.observedTripleIndices.size()) {
                       inverseTriplePatternInstances[j]->push_back(patternInstance); 
                    }
                }
                seenInstances.insert(direct_pattern_instance_concise_description);
            }
        } else {
            for (int j = 0; j <= nTriplesPerPattern; j++) {
                if (j <= patternInstance.observedTripleIndices.size()) {
                    inverseTriplePatternInstances[j]->push_back(patternInstance); 
                }
            }
        }
    };

	for (INT i = 0; i < trainTotal; i++) { // Reading train samples
        Triple triple = trainList[i];

        INT relation = enable_filters ? internal_to_external_relation_id.at(triple.r) : triple.r;
        // INT relation = triple.r;

        // cout << "Relation: " << relation << endl;

        auto forwardRelationIterator = inverseForwardRelationToBackward.find(relation); // unordered_map<INT, unordered_set<INT>>
        
        if (forwardRelationIterator != inverseForwardRelationToBackward.end()) {
            auto direct_pattern_instance = InversePatternInstance(
                    triple,
                    Triple(triple.t, enable_filters ? external_to_internal_relation_id.find(forwardRelationIterator->second)->second : forwardRelationIterator->second, triple.h)
            );
            pushPatternInstance(direct_pattern_instance);
        } else {
            auto backwardRelationIterator = inverseBackwardRelationToForward.find(triple.r); // unordered_map<INT, unordered_set<INT>>
            if (backwardRelationIterator != inverseBackwardRelationToForward.end()) {
                auto inverse_pattern_instance = InversePatternInstance(
                        Triple(triple.t, backwardRelationIterator->second, triple.h),
                        triple,
                        false
                );
                pushPatternInstance(inverse_pattern_instance);
            }
        }
	}

    if (verbose) {
        for (int i = 0; i <= nTriplesPerPattern; i++) {
            cout << "Collected " << inverseTriplePatternInstances[i]->size() << " inverse pattern instances in which there are " << i << " or more observed patterns" << endl;
        }

        std::cout << std::endl;
        // std::cout << std::endl << std::endl;
        // 
        // for (PatternInstance patternInstance: **inverseTriplePatternInstances) {
        //     ((InversePatternInstance*)&patternInstance)->print();
        // }
    }

    patternDescriptions[inversePatternName] = PatternDescription(inverse, nTriplesPerPattern, inverseTriplePatternInstances);
}

