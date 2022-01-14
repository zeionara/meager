#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <regex>
#include <stdexcept>

// #include "main.h"
#include "../Reader.h"

using namespace std;

// const int nTriplesPerPattern = 2;

// vector<PatternInstance> symmetricTriples;
// vector<PatternInstance>** inverseTriplePatternInstances = (vector<PatternInstance>**)malloc(sizeof(vector<PatternInstance>*) * (nTriplesPerPattern + 1));

vector<regex> readFilterPatterns(string filename, bool verbose, bool drop_duplicates) {
    if (verbose) {
		cout << "Reading " << filename << " triple patterns..." << endl;
    }

    string path = inPath + "filters/" + filename + ".txt";
    ifstream in_file(path);

    if (!in_file.good()) {
        throw invalid_argument("Cannot find file " + path + " on disk");
    }

    unordered_set<string> seenPatterns;
    vector<regex> patterns;

    // int forwardRelation, backwardRelation;
    // unordered_set<int> inverseForwardRelations;
    // unordered_set<int> inverseBackwardRelations;

    // unordered_map<int, int> inverseForwardRelationToBackward;
    // unordered_map<int, int> inverseBackwardRelationToForward;

    for (string line; getline(in_file, line);) {
        if (drop_duplicates) {
            if (seenPatterns.find(line) == seenPatterns.end()) {
                seenPatterns.insert(line);
                regex line_regex(line);
                patterns.push_back(line_regex); 
                cout << "Read filtering pattern " << line << endl;
            }
        } else {
            regex line_regex(line);
            patterns.push_back(line_regex); 
        }
    }

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

    // unordered_set<string> seenInstances;

    // for (int i = 0; i <= nTriplesPerPattern; i++) {
    //     inverseTriplePatternInstances[i] = new vector<PatternInstance>;
    // }

    // auto pushPatternInstance = [drop_duplicates, &seenInstances](PatternInstance patternInstance) {
    //     if (drop_duplicates) {
    //         string direct_pattern_instance_concise_description = patternInstance.getConciseDescription();
    //         if (seenInstances.find(direct_pattern_instance_concise_description) == seenInstances.end()) {
    //             for (int j = 0; j <= nTriplesPerPattern; j++) {
    //                 if (j <= patternInstance.observedTripleIndices.size()) {
    //                    inverseTriplePatternInstances[j]->push_back(patternInstance); 
    //                 }
    //             }
    //             seenInstances.insert(direct_pattern_instance_concise_description);
    //         }
    //     } else {
    //         for (int j = 0; j <= nTriplesPerPattern; j++) {
    //             if (j <= patternInstance.observedTripleIndices.size()) {
    //                 inverseTriplePatternInstances[j]->push_back(patternInstance); 
    //             }
    //         }
    //     }
    // };

	// for (INT i = 0; i < trainTotal; i++) { // Reading train samples
    //     Triple triple = trainList[i];

    //     auto forwardRelationIterator = inverseForwardRelationToBackward.find(triple.r); // unordered_map<INT, unordered_set<INT>>
    //     
    //     if (forwardRelationIterator != inverseForwardRelationToBackward.end()) {
    //         auto direct_pattern_instance = InversePatternInstance(
    //                 triple,
    //                 Triple(triple.t, forwardRelationIterator->second, triple.h)
    //         );
    //         pushPatternInstance(direct_pattern_instance);
    //     } else {
    //         auto backwardRelationIterator = inverseBackwardRelationToForward.find(triple.r); // unordered_map<INT, unordered_set<INT>>
    //         if (backwardRelationIterator != inverseBackwardRelationToForward.end()) {
    //             auto inverse_pattern_instance = InversePatternInstance(
    //                     Triple(triple.t, backwardRelationIterator->second, triple.h),
    //                     triple,
    //                     false
    //             );
    //             pushPatternInstance(inverse_pattern_instance);
    //         }
    //     }
	// }

    // if (verbose) {
    //     for (int i = 0; i <= nTriplesPerPattern; i++) {
    //         cout << "Collected " << inverseTriplePatternInstances[i]->size() << " inverse pattern instances in which there are " << i << " or more observed patterns" << endl;
    //     }

    //     std::cout << std::endl;
    //     // std::cout << std::endl << std::endl;
    //     // 
    //     // for (PatternInstance patternInstance: **inverseTriplePatternInstances) {
    //     //     ((InversePatternInstance*)&patternInstance)->print();
    //     // }
    // }

    // patternDescriptions[inversePatternName] = PatternDescription(inverse, nTriplesPerPattern, inverseTriplePatternInstances);

    return patterns;
}

