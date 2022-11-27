#ifndef TRIPLE_LIST_THIN_TRIPLE_LIST_WRAPPER_H
#define TRIPLE_LIST_THIN_TRIPLE_LIST_WRAPPER_H

#include "../main.h"
#include "../reader.h"
#include "../TripleIndex.h"
#include "../../utils/main.h"

#include "TripleList.h"
#include "../TripleEncoder.h"
#include "../../filters/TripleFilter.h"

template <typename T>
struct ThinTripleListWrapper {
    TripleList* content;

    INT length;

    TripleIndex* index;

    // INT nEntities;
    // INT nRelations;

    ThinTripleListWrapper(SubsetType subset, INT startInternalEntityId, INT startInternalRelationId, bool enable_filters = false, bool verbose = false) {
        File* file = readNumberOfTriples(subset, "", verbose);

        this->content = new TripleList(file->length, ::TripleElement::rel);

        this->length = file->length;
        this->index = new TripleIndex;

        // this->read(file, startInternalEntityId, startInternalRelationId, enable_filters, verbose);
    }

    ThinTripleListWrapper(SubsetType subset, CorpusReader<T>* reader, TripleFilter<T>* filter, TripleEncoder<INT>* encoder, bool enable_filters, bool verbose = false) {
        this->index = new TripleIndex;

        cout << "reading triples" << endl;
        this->content = reader->readTriples(subset, index, ::TripleElement::rel, filter, encoder, verbose);
        cout << "finished reading triples" << endl;

        this->length = content->length;

        this->read(reader, filter, encoder, enable_filters, verbose);
    }

    void sort(INT nEntities) {
        this->content->sort(nEntities);
    }

    // void read(File* file, INT startInternalEntityId, INT startInternalRelationId, bool enable_filters = false, bool verbose = false) {
    void read(CorpusReader<T>* reader, TripleFilter<T>* filter, TripleEncoder<INT>* encoder, bool enable_filters, bool verbose = false) {
        // INT nTriples = readTriples(file, enable_filters, filter, encoder, this->content->items, this->index);

        if (enable_filters) {
            this->sort(encoder->entity->nEncodedValues);
        } else {
            this->sort(reader->readVocabularySize(entity));
        }
    }
};

#endif
