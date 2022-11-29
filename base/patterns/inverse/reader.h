#ifndef INVERSE_PATTERN_READER
#define INVERSE_PATTERN_READER

#include <cstdlib>

#include "../main.h"

#include "../../triple/TripleIndex.h"
#include "../../storage/CorpusReader.h"

#include "main.h"

// template <typename T>
// void separateInverseTriples(string path, Triple* triples, INT nTriples, TripleIndex* index, CorpusReader<T>* reader, bool verbose = false, bool drop_duplicates = true, bool enable_filters = false);
// template <typename T>
// void separateInverseTriples(string path, Triple* triples, INT nTriples, TripleIndex* index, CorpusReader<T>* reader, bool verbose, bool drop_duplicates, bool enable_filters);

// extern
// std::vector<PatternInstance> symmetricTriples;
extern
std::vector<PatternInstance>** inverseTriplePatternInstances;

const int nTriplesPerInversePattern = 2;

template <typename T>
void separateInverseTriples(string path, Triple* triples, INT nTriples, TripleIndex* index, CorpusReader<T>* reader, bool verbose, bool drop_duplicates, bool enable_filters) {
    BinaryPatternRelationMap<T>* relationMap = reader->readBinaryPatterns(inverse, true);
    
    // if (verbose) {
	// 	cout << "Separating inverse triples..." << endl;
    // }

    // ifstream in_file(path + "patterns/inverse.txt");

    // int forwardRelation, backwardRelation;
    // // unordered_set<int> inverseForwardRelations;
    // // unordered_set<int> inverseBackwardRelations;

    // unordered_map<int, int> inverseForwardRelationToBackward;
    // unordered_map<int, int> inverseBackwardRelationToForward;
    unordered_map<INT, INT> inverseForwardRelationToBackward = relationMap->firstToSecond;
    unordered_map<INT, INT> inverseBackwardRelationToForward = relationMap->secondToFirst;

    // // cout << forwardRelation << endl;
    // // cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

    // while (in_file >> forwardRelation >> backwardRelation) {
    //     if (verbose) {
    //         printf("Read relations %d and %d.\n", forwardRelation, backwardRelation);
    //     }

    //     // inverseForwardRelations.insert(forwardRelation);
    //     // inverseBackwardRelations.insert(backwardRelation);

    //     inverseForwardRelationToBackward[forwardRelation] = backwardRelation;
    //     inverseBackwardRelationToForward[backwardRelation] = forwardRelation;
    // }
    // 
    // if (verbose) {
    //     printf("Number of inverse relations = %d.\n", (int)inverseForwardRelationToBackward.size());
    // }

    // in_file.close();

    // if (verbose) {
    //     cout << "Inverse triples" << endl;
    // }

    unordered_set<string> seenInstances;

    for (int i = 0; i <= nTriplesPerInversePattern; i++) {
        inverseTriplePatternInstances[i] = new vector<PatternInstance>;
    }

    auto pushPatternInstance = [drop_duplicates, &seenInstances](PatternInstance patternInstance) {
        if (drop_duplicates) {
            string direct_pattern_instance_concise_description = patternInstance.getConciseDescription();
            if (seenInstances.find(direct_pattern_instance_concise_description) == seenInstances.end()) {
                for (int j = 0; j <= nTriplesPerInversePattern; j++) {
                    if (j <= patternInstance.observedTripleIndices.size()) {
                       inverseTriplePatternInstances[j]->push_back(patternInstance); 
                    }
                }
                seenInstances.insert(direct_pattern_instance_concise_description);
            }
        } else {
            for (int j = 0; j <= nTriplesPerInversePattern; j++) {
                if (j <= patternInstance.observedTripleIndices.size()) {  // if at least j triples from the pattern are observed
                    inverseTriplePatternInstances[j]->push_back(patternInstance); 
                }
            }
        }
    };

    cout << "Still processing inverse pattern" << endl;

	for (INT i = 0; i < nTriples; i++) { // Reading train samples
        Triple triple = triples[i];

        // INT relation = enable_filters ? internal_to_external_relation_id.at(triple.r) : triple.r;
        // keep relations and entities represented using internal indices
        INT relation = triple.r;

        // cout << "Relation: " << relation << endl;

        auto forwardRelationIterator = inverseForwardRelationToBackward.find(relation); // unordered_map<INT, unordered_set<INT>>
        
        if (forwardRelationIterator != inverseForwardRelationToBackward.end()) {
            // cout << "1" << endl;
            // cout << triple.t << endl;
            auto direct_pattern_instance = InversePatternInstance(
                triple,
                // Triple(triple.t, enable_filters ? external_to_internal_relation_id.find(forwardRelationIterator->second)->second : forwardRelationIterator->second, triple.h),
                Triple(triple.t, forwardRelationIterator->second, triple.h),
                index
            );
            // cout << "Pushing pattern instance" << endl;
            pushPatternInstance(direct_pattern_instance);
            // cout << "Pushed pattern instance" << endl;
        } else {
            // cout << "2" << endl;
            auto backwardRelationIterator = inverseBackwardRelationToForward.find(triple.r); // unordered_map<INT, unordered_set<INT>>
            if (backwardRelationIterator != inverseBackwardRelationToForward.end()) {
                auto inverse_pattern_instance = InversePatternInstance(
                    Triple(triple.t, backwardRelationIterator->second, triple.h),
                    triple,
                    index,
                    false
                );
                // cout << "Pushing pattern instance" << endl;
                pushPatternInstance(inverse_pattern_instance);
                // cout << "Pushed pattern instance" << endl;
            }
        }
	}

    if (verbose) {
        for (int i = 0; i <= nTriplesPerInversePattern; i++) {
            cout << "Collected " << inverseTriplePatternInstances[i]->size() << " inverse pattern instances in which there are " << i << " or more observed patterns" << endl;
        }

        std::cout << std::endl;
        // std::cout << std::endl << std::endl;
        // 
        // for (PatternInstance patternInstance: **inverseTriplePatternInstances) {
        //     ((InversePatternInstance*)&patternInstance)->print();
        // }
    }

    // patternDescriptions[inverse] = PatternDescription(inverse, nTriplesPerInversePattern, inverseTriplePatternInstances);
}

#endif

