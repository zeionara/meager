#ifndef STORAGE_CORPUS_READER_H
#define STORAGE_CORPUS_READER_H

#include <vector>
#include <regex>

#include "../triple/TripleEncoder.h"
#include "../triple/list/TripleList.h"
#include "../triple/TripleIndex.h"
#include "../utils/main.h"

using namespace std;

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

    virtual TripleList* readTriples(SubsetType subsetType, TripleIndex* tripleIndex, TripleElement tripleElement, TripleFilter<T>* filter, TripleEncoder<T>* encoder, bool verbose) = 0;
    virtual INT readVocabularySize(TripleComponent tripleComponent, bool verbose = false) = 0;
    virtual vector<regex> readFilterPatterns(bool excluding = false, bool verbose = false, bool drop_duplicates = true) = 0;
    virtual RelationTypesContents<T>* readRelationTypesContents(bool verbose = false) = 0;

};

#endif
