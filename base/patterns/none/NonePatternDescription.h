#ifndef PATTERNS_NONE_PATTERN_DESCRIPTION_H
#define PATTERNS_NONE_PATTERN_DESCRIPTION_H

#include "NonePatternInstance.h"

#include "../../triple/TripleIndex.h"
#include "../../triple/list/TripleList.h"

struct NonePatternDescription: PatternDescription {

    NonePatternDescription(TripleList* triples, bool verbose = false, bool dropDuplicates = true) {

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

        cout << "Should collect " << triples->length << " none pattern instances" << endl;

        if (verbose) {
            for (int i = 0; i <= nTriplesPerInstance; i++) {
                cout << "Collected " << instanceSets[i]->size() << " none pattern instances in which there are " << i << " or more observed patterns" << endl;
            }
        }

    }

};

#endif
