#ifndef PATTERNS_INVERSE_PATTERN_DESCRIPTION_H
#define PATTERNS_INVERSE_PATTERN_DESCRIPTION_H

#include "../../storage/reader/CorpusReader.h"

#include "../BinaryPatternRelationMap.h"
#include "../PatternDescription.h"

#include "InversePatternInstance.h"
#include "../PatternDescriptionTemplates.h"

template <typename T>
struct InversePatternDescription: PatternDescription {

    InversePatternDescription(
        triple::List* triples, BinaryPatternDescriptionTemplate<T>* descriptionTemplate, triple::Index* index, triple::Encoder<T>* encoder,
        bool dropDuplicates = true, bool enableFilters = false, bool verbose = false
    ) {

        label = inversePatternName;
        id = inverse;
        nTriplesPerInstance = 2; 

        initInstanceLists();

        BinaryPatternRelationMap<T>* relationMap = descriptionTemplate->content;
        
        auto inverseForwardRelationToBackward = relationMap->firstToSecond;
        auto inverseBackwardRelationToForward = relationMap->secondToFirst;

        unordered_set<string> seenInstances;

        for (INT i = 0; i < triples->length; i++) { // Reading train samples
            triple::Triple triple = triples->items[i];

            INT relation = triple.r;

            auto forwardRelationIterator = inverseForwardRelationToBackward.find(relation);
            
            if (forwardRelationIterator != inverseForwardRelationToBackward.end()) {
                tryPush(
                    InversePatternInstance(
                        triple,
                        triple::Triple(triple.t, forwardRelationIterator->second, triple.h),
                        index
                    ),
                    seenInstances, dropDuplicates
                );
            } else {
                auto backwardRelationIterator = inverseBackwardRelationToForward.find(triple.r);

                if (backwardRelationIterator != inverseBackwardRelationToForward.end()) {
                    tryPush(
                        InversePatternInstance(
                            triple::Triple(triple.t, backwardRelationIterator->second, triple.h),
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
