#ifndef PATTERNS_SYMMETRIC_PATTERN_DESCRIPTION_H
#define PATTERNS_SYMMETRIC_PATTERN_DESCRIPTION_H

#include "../../storage/CorpusReader.h"

#include "../BinaryPatternRelationMap.h"
#include "../PatternDescription.h"

#include "SymmetricPatternInstance.h"

template <typename T>
struct SymmetricPatternDescription: PatternDescription {

    SymmetricPatternDescription(
        TripleList* triples, CorpusReader<T>* reader, TripleIndex* index, TripleEncoder<T>* encoder,
        bool dropDuplicates = true, bool enableFilters = false, bool verbose = false
    ) {

        label = symmetricPatternName;
        id = symmetric;
        nTriplesPerInstance = 2;

        initInstanceLists();

        UnaryPatternRelationSet<T>* relationSet = reader->readUnaryPatterns(symmetric, encoder, enableFilters, verbose);
        
        auto relations = relationSet->relations;

        unordered_set<string> seenInstances;

        for (INT i = 0; i < triples->length; i++) { // Reading train samples
            Triple triple = triples->items[i];

            if (relations.find(triple.r) != relations.end()) {
                tryPush(
                    SymmetricPatternInstance(
                        triple,
                        Triple(triple.t, triple.r, triple.h),
                        index
                    ),
                    seenInstances, dropDuplicates
                );

                tryPush(
                    SymmetricPatternInstance(
                        Triple(triple.t, triple.r, triple.h),
                        triple,
                        index,
                        false
                    ),
                    seenInstances, dropDuplicates
                );
            }
        }

        seenInstances.clear();

        if (verbose) {
            printSummary();
        }
    }
};

#endif
