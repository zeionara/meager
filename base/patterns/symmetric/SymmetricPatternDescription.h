#ifndef PATTERNS_SYMMETRIC_PATTERN_DESCRIPTION_H
#define PATTERNS_SYMMETRIC_PATTERN_DESCRIPTION_H

#include "../BinaryPatternRelationMap.h"
#include "../../storage/CorpusReader.h"
#include "main.h"

template <typename T>
struct SymmetricPatternDescription: PatternDescription {
    Pattern id;
    int nTriplesPerInstance;
    vector<PatternInstance>** instanceSets;

    SymmetricPatternDescription(TripleList* triples, CorpusReader<T>* reader, TripleIndex* index, bool verbose = false, bool dropDuplicates = true) {
        INT nTriplesPerInstance_ = 2;

        vector<PatternInstance>** instances = (vector<PatternInstance>**)malloc(sizeof(vector<PatternInstance>*) * (nTriplesPerInstance_ + 1));

        UnaryPatternRelationSet<T>* relationSet = reader->readUnaryPatterns(symmetric, true);
        
        unordered_set<INT> relations = relationSet->relations;

        unordered_set<string> seenInstances;

        for (int i = 0; i <= nTriplesPerInstance_; i++) {
            instances[i] = new vector<PatternInstance>;
        }

        auto pushPatternInstance = [dropDuplicates, &seenInstances, &instances, nTriplesPerInstance_](PatternInstance patternInstance) {
            if (dropDuplicates) {
                // cout << "pushing found triple" << endl;
                string direct_pattern_instance_concise_description = patternInstance.getConciseDescription();
                if (seenInstances.find(direct_pattern_instance_concise_description) == seenInstances.end()) {
                    // symmetricTriples.push_back(
                    //         patternInstance
                    // );
                    for (int j = 0; j <= nTriplesPerInstance_; j++) {
                        if (j <= patternInstance.observedTripleIndices.size()) {
                           instances[j]->push_back(patternInstance); 
                        }
                    }
                    seenInstances.insert(direct_pattern_instance_concise_description);
                }
            } else {
                    // symmetricTriples.push_back(
                    //         patternInstance
                    // );
                    for (int j = 0; j <= nTriplesPerInstance_; j++) {
                        if (j <= patternInstance.observedTripleIndices.size()) {
                           instances[j]->push_back(patternInstance); 
                        }
                    }
            }
        };

        for (INT i = 0; i < triples->length; i++) { // Reading train samples
            Triple triple = triples->items[i];

            if (relations.find(triple.r) != relations.end()) {
                // cout << triple.r << " is a symmetric relation" << endl;
                auto direct_pattern_instance = SymmetricPatternInstance(
                        triple,
                        Triple(triple.t, triple.r, triple.h),
                        index
                );
                pushPatternInstance(direct_pattern_instance);

                auto inverse_pattern_instance = SymmetricPatternInstance(
                    Triple(triple.t, triple.r, triple.h),
                    triple,
                    index,
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
            for (int i = 0; i <= nTriplesPerInstance_; i++) {
                cout << "Collected " << instances[i]->size() << " symmetric pattern instances in which there are " << i << " or more observed patterns" << endl;
            }

            std::cout << std::endl;
            // std::cout << std::endl << std::endl;
            // 
            // for (PatternInstance patternInstance: **symmetricTriplePatternInstances) {
            //     ((SymmetricPatternInstance*)&patternInstance)->print();
            // }
        }
        // unordered_set<string> seenInstances;

        // for (int i = 0; i <= nTriplesPerInstance_; i++) {
        //     instances[i] = new vector<PatternInstance>;
        // }

        // auto pushPatternInstance = [dropDuplicates, &seenInstances, &instances, nTriplesPerInstance_](PatternInstance patternInstance) {
        //     if (dropDuplicates) {
        //         string direct_pattern_instance_concise_description = patternInstance.getConciseDescription();
        //         if (seenInstances.find(direct_pattern_instance_concise_description) == seenInstances.end()) {
        //             for (int j = 0; j <= nTriplesPerInstance_; j++) {
        //                 if (j <= patternInstance.observedTripleIndices.size()) {
        //                    instances[j]->push_back(patternInstance); 
        //                 }
        //             }
        //             seenInstances.insert(direct_pattern_instance_concise_description);
        //         }
        //     } else {
        //         for (int j = 0; j <= nTriplesPerInstance_; j++) {
        //             if (j <= patternInstance.observedTripleIndices.size()) {  // if at least j triples from the pattern are observed
        //                 instances[j]->push_back(patternInstance); 
        //             }
        //         }
        //     }
        // };

        // cout << "Still processing inverse pattern" << endl;

        // for (INT i = 0; i < triples->length; i++) { // Reading train samples
        //     Triple triple = triples->items[i];

        //     // INT relation = enable_filters ? internal_to_external_relation_id.at(triple.r) : triple.r;
        //     // keep relations and entities represented using internal indices
        //     INT relation = triple.r;

        //     // cout << "Relation: " << relation << endl;

        //     auto forwardRelationIterator = inverseForwardRelationToBackward.find(relation); // unordered_map<INT, unordered_set<INT>>
        //     
        //     if (forwardRelationIterator != inverseForwardRelationToBackward.end()) {
        //         // cout << "1" << endl;
        //         // cout << triple.t << endl;
        //         auto direct_pattern_instance = InversePatternInstance(
        //             triple,
        //             // Triple(triple.t, enable_filters ? external_to_internal_relation_id.find(forwardRelationIterator->second)->second : forwardRelationIterator->second, triple.h),
        //             Triple(triple.t, forwardRelationIterator->second, triple.h),
        //             index
        //         );
        //         // cout << "Pushing pattern instance" << endl;
        //         pushPatternInstance(direct_pattern_instance);
        //         // cout << "Pushed pattern instance" << endl;
        //     } else {
        //         // cout << "2" << endl;
        //         auto backwardRelationIterator = inverseBackwardRelationToForward.find(triple.r); // unordered_map<INT, unordered_set<INT>>
        //         if (backwardRelationIterator != inverseBackwardRelationToForward.end()) {
        //             auto inverse_pattern_instance = InversePatternInstance(
        //                 Triple(triple.t, backwardRelationIterator->second, triple.h),
        //                 triple,
        //                 index,
        //                 false
        //             );
        //             // cout << "Pushing pattern instance" << endl;
        //             pushPatternInstance(inverse_pattern_instance);
        //             // cout << "Pushed pattern instance" << endl;
        //         }
        //     }
        // }

        // if (verbose) {
        //     for (int i = 0; i <= nTriplesPerInstance_; i++) {
        //         cout << "Collected " << instances[i]->size() << " inverse pattern instances in which there are " << i << " or more observed patterns" << endl;
        //     }

        //     std::cout << std::endl;
        //     // std::cout << std::endl << std::endl;
        //     // 
        //     // for (PatternInstance patternInstance: **inverseTriplePatternInstances) {
        //     //     ((InversePatternInstance*)&patternInstance)->print();
        //     // }
        // }

        id = none;
        nTriplesPerInstance = 2; 
        instanceSets = instances;
    }
};

#endif
