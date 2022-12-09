#ifndef TRIPLE_LIST_THIN_TRIPLE_LIST_WRAPPER_H
#define TRIPLE_LIST_THIN_TRIPLE_LIST_WRAPPER_H

#include "../main.h"
#include "../TripleIndex.h"
// #include "../../utils/main.h"

#include "TripleList.h"
#include "../TripleEncoder.h"
#include "../../filters/TripleFilter.h"

using namespace meager::main::storage;

template <typename T>
struct ThinTripleListWrapper {
    TripleList* content;

    INT length;

    TripleIndex* index;

    ThinTripleListWrapper(SubsetType subset, reader::Corpus<T>* reader, TripleFilter<T>* filter, TripleEncoder<INT>* encoder, bool enableFilters, bool verbose = false) {
        this->index = new TripleIndex();

        if (verbose) {
            cout << "started reading triples" << endl;
        }

        this->content = reader->readTriples(subset, index, ::TripleElement::rel, filter, encoder, enableFilters, verbose);

        if (verbose) {
            cout << "finished reading triples" << endl;
        }

        this->length = content->length;

        this->read(reader, filter, encoder, enableFilters, verbose);
    }

    void sort(INT nRelations) {
        this->content->sort(nRelations); // Sort by relation, then by head, then by tail
    }

    void read(reader::Corpus<T>* reader, TripleFilter<T>* filter, TripleEncoder<INT>* encoder, bool enableFilters, bool verbose = false) {
        if (enableFilters) {
            this->sort(encoder->relation->nEncodedValues);
        } else {
            this->sort(reader->readVocabularySize(::TripleComponent::relation));
        }
    }
};

#endif
