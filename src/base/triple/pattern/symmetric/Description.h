#ifndef TRIPLE_PATTERN_SYMMETRIC_DESCRIPTION_H
#define TRIPLE_PATTERN_SYMMETRIC_DESCRIPTION_H

// #include "../../../storage/reader/CorpusReader.h"

// #include "../relation/BinaryMap.h"
#include "../description/Description.h"
#include "../description/Templates.h"

#include "Instance.h"

namespace meager::main::triple::pattern::symmetric {

    template <typename T>
    struct Description: description::Description {

        Description(
            triple::list::List* triples, description::UnaryTemplate<T>* descriptionTemplate, triple::Index* index, triple::Encoder<T>* encoder,
            bool dropDuplicates = true, bool enableFilters = false, bool verbose = false
        ) {

            label = symmetric::name;
            id = Pattern::Symmetric;
            nTriplesPerInstance = 2;

            initInstanceLists();

            relation::UnarySet<T>* relationSet = descriptionTemplate->content;
            
            auto relations = relationSet->relations;

            unordered_set<string> seenInstances;

            for (INT i = 0; i < triples->length; i++) { // Reading train samples
                triple::Triple triple = triples->items[i];

                if (relations.find(triple.r) != relations.end()) {
                    tryPush(
                        symmetric::Instance(
                            triple,
                            triple::Triple(triple.t, triple.r, triple.h),
                            index
                        ),
                        seenInstances, dropDuplicates
                    );

                    tryPush(
                        symmetric::Instance(
                            triple::Triple(triple.t, triple.r, triple.h),
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

}

#endif
