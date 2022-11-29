#ifndef PATTERNS_NONE_PATTERN_DESCRIPTION_H
#define PATTERNS_NONE_PATTERN_DESCRIPTION_H

#include "NonePatternInstance.h"

#include "../../triple/TripleIndex.h"
#include "../../triple/list/TripleList.h"

struct NonePatternDescription: PatternDescription {

    NonePatternDescription(TripleList* triples, bool verbose = false, bool dropDuplicates = true) {

        label = nonePatternName;
        id = none;
        nTriplesPerInstance = 1;

        initInstanceLists();

        unordered_set<string> seenInstances;

        for (INT i = 0; i < triples->length; i++) { // Reading train samples
            tryPush(
                NonePatternInstance(triples->items[i]),
                seenInstances, dropDuplicates
            );
        }

        seenInstances.clear();

        if (verbose) {
            printSummary(triples->length);
        }

    }

};

#endif
