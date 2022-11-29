#ifndef PATTERNS_INVERSE_PATTERN_DESCRIPTION_H
#define PATTERNS_INVERSE_PATTERN_DESCRIPTION_H

#include "../BinaryPatternRelationMap.h"
#include "../../storage/CorpusReader.h"

#include "InversePatternInstance.h"

template <typename T>
struct InversePatternDescription: PatternDescription {

    InversePatternDescription(TripleList* triples, CorpusReader<T>* reader, TripleIndex* index, bool verbose = false, bool dropDuplicates = true) {

        label = inversePatternName;
        id = inverse;
        nTriplesPerInstance = 2; 

        initInstanceLists();

        BinaryPatternRelationMap<T>* relationMap = reader->readBinaryPatterns(inverse, verbose);
        
        auto inverseForwardRelationToBackward = relationMap->firstToSecond;
        auto inverseBackwardRelationToForward = relationMap->secondToFirst;

        unordered_set<string> seenInstances;

        for (INT i = 0; i < triples->length; i++) { // Reading train samples
            Triple triple = triples->items[i];

            INT relation = triple.r;

            auto forwardRelationIterator = inverseForwardRelationToBackward.find(relation);
            
            if (forwardRelationIterator != inverseForwardRelationToBackward.end()) {
                tryPush(
                    InversePatternInstance(
                        triple,
                        Triple(triple.t, forwardRelationIterator->second, triple.h),
                        index
                    ),
                    seenInstances, dropDuplicates
                );
            } else {
                auto backwardRelationIterator = inverseBackwardRelationToForward.find(triple.r);

                if (backwardRelationIterator != inverseBackwardRelationToForward.end()) {
                    tryPush(
                        InversePatternInstance(
                            Triple(triple.t, backwardRelationIterator->second, triple.h),
                            triple,
                            index,
                            false
                        ),
                        seenInstances, dropDuplicates
                    );
                }
            }
        }

        seenInstances.clear();

        if (verbose) {
            printSummary();
        }

    }

};

#endif
