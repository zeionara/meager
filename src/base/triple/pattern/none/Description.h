#ifndef TRIPLE_PATTERN_NONE_DESCRIPTION_H
#define TRIPLE_PATTERN_NONE_DESCRIPTION_H

#include "../../list/List.h"

#include "../description/Description.h"

#include "Instance.h"

namespace meager::main::triple::pattern::description {

    struct NonePatternDescription: Description {

        NonePatternDescription(triple::list::List* triples, bool verbose = false, bool dropDuplicates = true) {

            label = none::name;
            id = Pattern::None;
            nTriplesPerInstance = 1;

            initInstanceLists();

            unordered_set<string> seenInstances;

            for (INT i = 0; i < triples->length; i++) { // Reading train samples
                tryPush(
                    none::Instance(triples->items[i]),
                    seenInstances, dropDuplicates
                );
            }

            seenInstances.clear();

            if (verbose) {
                printSummary(triples->length);
            }

        }

    };

}

#endif
