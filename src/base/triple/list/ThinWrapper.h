#ifndef TRIPLE_LIST_THIN_TRIPLE_LIST_WRAPPER_H
#define TRIPLE_LIST_THIN_TRIPLE_LIST_WRAPPER_H

// #include "../main.h"
// #include "../TripleIndex.h"
// #include "../../utils/main.h"

// #include "TripleList.h"
// #include "../TripleEncoder.h"
#include "../../filters/TripleFilter.h"

using namespace meager::main::storage;

namespace meager::main::triple::list {

    template <typename T>
    struct ThinWrapper {
        List* content;

        INT length;

        Index* index;

        ThinWrapper(SubsetType subset, reader::Corpus<T>* reader, TripleFilter<T>* filter, Encoder<INT>* encoder, bool enableFilters, bool verbose = false) {
            this->index = new Index();

            if (verbose) {
                cout << "started reading triples" << endl;
            }

            this->content = reader->readTriples(subset, index, Component::relation, filter, encoder, enableFilters, verbose);

            if (verbose) {
                cout << "finished reading triples" << endl;
            }

            this->length = content->length;

            if (enableFilters) {
                this->sort(encoder->relation->nEncodedValues);
            } else {
                this->sort(reader->readVocabularySize(ComponentType::relation));
            }
        }

        void sort(INT nRelations) {
            this->content->sort(nRelations); // Sort by relation, then by head, then by tail
        }

    };

}

#endif
