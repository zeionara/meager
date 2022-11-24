#ifndef TRIPLE_LIST_THIN_TRIPLE_LIST_WRAPPER_H
#define TRIPLE_LIST_THIN_TRIPLE_LIST_WRAPPER_H

#include "../main.h"
#include "../reader.h"
#include "../TripleIndex.h"
#include "../../utils/main.h"

#include "TripleList.h"
#include "../TripleEncoder.h"
#include "../../filters/TripleFilter.h"

struct ThinTripleListWrapper {
    TripleList* content;

    INT length;

    TripleIndex* index;

    // INT nEntities;
    // INT nRelations;

    ThinTripleListWrapper(SubsetType subset, INT startInternalEntityId, INT startInternalRelationId, bool enable_filters = false, bool verbose = false) {
        File* file = readNumberOfTriples(subset, verbose);

        this->content = new TripleList(file->length, ::TripleElement::rel);

        this->length = file->length;
        this->index = new TripleIndex;

        // this->read(file, startInternalEntityId, startInternalRelationId, enable_filters, verbose);
    }

    ThinTripleListWrapper(string path, bool enable_filters, TripleFilter* filter, TripleEncoder* encoder, bool verbose = false) {
        File* file = readNumberOfTriples(path, verbose);

        this->content = new TripleList(file->length, ::TripleElement::rel);

        this->length = file->length;
        this->index = new TripleIndex;

        this->read(file, enable_filters, filter, encoder, verbose);
    }

    void sort(INT nEntities) {
        this->content->sort(nEntities);
    }

    // void read(File* file, INT startInternalEntityId, INT startInternalRelationId, bool enable_filters = false, bool verbose = false) {
    void read(File* file, bool enable_filters, TripleFilter* filter, TripleEncoder* encoder, bool verbose = false) {
        cout << "reading triples" << endl;
        INT nTriples = readTriples(file, enable_filters, filter, encoder, this->content->items, this->index);
        cout << "finished reading triples" << endl;

        file->close();

        if (enable_filters) {
            this->sort(encoder->entity->nEncodedValues);
        } else {
            File* entities = readNumberOfElements(TripleComponent::entity, verbose);
            this->sort(entities->length);
        }
    }
};

#endif
