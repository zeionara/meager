#ifndef TRIPLE_LIST_THIN_TRIPLE_LIST_WRAPPER_H
#define TRIPLE_LIST_THIN_TRIPLE_LIST_WRAPPER_H

#include "../main.h"
#include "../reader.h"
#include "../TripleIndex.h"
#include "../../utils/main.h"

#include "TripleList.h"

struct ThinTripleListWrapper {
    TripleList* content;

    INT length;

    TripleIndex* index;

    INT nEntities;
    INT nRelations;

    ThinTripleListWrapper(SubsetType subset, INT startInternalEntityId, INT startInternalRelationId, bool enable_filters = false, bool verbose = false) {
        File* file = readNumberOfTriples(subset, verbose);

        this->content = new TripleList(file->length, ::TripleElement::rel);

        this->length = file->length;
        this->index = new TripleIndex;

        this->read(file, startInternalEntityId, startInternalRelationId, enable_filters, verbose);
    }

    void sort() {
        this->content->sort(this->nRelations);
    }

    void read(File* file, INT startInternalEntityId, INT startInternalRelationId, bool enable_filters = false, bool verbose = false) {
        TripleIds tripleIds = readTriples(file, enable_filters, this->content->items, this->index, startInternalEntityId, startInternalRelationId);

        file->close();

        this->nEntities = tripleIds.last_entity;
        this->nRelations = tripleIds.last_relation;

        this->sort();
    }
};

#endif
