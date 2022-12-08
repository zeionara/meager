#ifndef STORAGE_CORPUS_READER_H
#define STORAGE_CORPUS_READER_H

#include <vector>
#include <regex>

#include "../../triple/TripleEncoder.h"
#include "../../triple/list/TripleList.h"
#include "../../triple/TripleIndex.h"
#include "../../utils/main.h"

#include "../../patterns/main.h"
#include "../../patterns/BinaryPatternRelationMap.h"
#include "../../patterns/UnaryPatternRelationSet.h"

using namespace std;
using namespace meager::main::utils;

template <typename T>
struct RelationTypeContents {
    INT relation;
    INT length;
    T* entities;

    RelationTypeContents(T* entities, INT length, INT relation) {
        this->relation = relation;
        this->length = length;
        this->entities = entities;
    }

    void encode(TripleEncoder<T>* encoder) {
        INT j = 0;

        relation = encoder->relation->encode(relation);
        for (INT i = 0; i < this->length; i++) {
            if (encoder->entity->contains(entities[i])) {
                entities[j++] = encoder->entity->encode(entities[i]);
            }
        }

        length = j;
    }
};

template <typename T>
struct RelationTypesContents {
    RelationTypeContents<T>** relations;
    INT length;

    RelationTypesContents(RelationTypeContents<T>** relations, INT length) {
        this->relations = relations;
        this->length = length;
    }
};

template <typename T>
struct TripleFilter;

template <typename T>
struct CorpusReader {

    string path; // TODO: Delete this

    virtual TripleList* readTriples(
        SubsetType subsetType, TripleIndex* tripleIndex, TripleElement tripleElement, TripleFilter<T>* filter, TripleEncoder<T>* encoder,
        bool enableFilters = false, bool verbose = false
    ) = 0;
    virtual INT readVocabularySize(TripleComponent tripleComponent, bool verbose = false) = 0;
    virtual vector<regex> readFilterPatterns(bool excluding = false, bool verbose = false, bool drop_duplicates = true) = 0;
    virtual RelationTypesContents<T>* readRelationTypesContents(bool verbose = false) = 0;

    virtual BinaryPatternRelationMap<INT>* readBinaryPatterns(Pattern pattern, TripleEncoder<T>* encoder, bool enableFilters = false, bool verbose = false) = 0;
    virtual UnaryPatternRelationSet<INT>* readUnaryPatterns(Pattern pattern, TripleEncoder<T>* encoder, bool enableFilters = false, bool verbose = false) = 0;
};

#endif
