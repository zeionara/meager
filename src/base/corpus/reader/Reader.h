#ifndef STORAGE_CORPUS_READER_H
#define STORAGE_CORPUS_READER_H

#include <vector>
#include <regex>

#include "../../triple/Encoder.h"
#include "../../triple/list/List.h"
#include "../../triple/Index.h"
#include "../../utils/main.h"

#include "../../triple/pattern/main.h"
#include "../../triple/pattern/relation/BinaryMap.h"
#include "../../triple/pattern/relation/UnarySet.h"

using namespace std;
using namespace meager::main;
using namespace meager::main::utils;

namespace meager::main::triple::relation {

    template <typename T>
    struct TypeContents {
        INT relation;
        INT length;
        T* entities;

        TypeContents(T* entities, INT length, INT relation) {
            this->relation = relation;
            this->length = length;
            this->entities = entities;
        }

        void encode(triple::Encoder<T>* encoder) {
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
    struct TypesContents {
        TypeContents<T>** relations;
        INT length;

        TypesContents(TypeContents<T>** relations, INT length) {
            this->relations = relations;
            this->length = length;
        }
    };

}

namespace meager::main::triple::filter {
    template <typename T>
    struct Filter;
}

namespace meager::main::corpus::reader {

    template <typename T>
    struct Corpus {

        string path; // TODO: Delete this

        virtual triple::list::List* readTriples(
            subset::Type subsetType, triple::Index* tripleIndex, triple::Component tripleComponent, triple::filter::Filter<T>* filter, triple::Encoder<T>* encoder,
            bool enableFilters = false, bool verbose = false
        ) = 0;

        virtual INT readVocabularySize(triple::ComponentType componentType, bool verbose = false) = 0;
        virtual vector<regex> readFilterPatterns(bool excluding = false, bool verbose = false, bool drop_duplicates = true) = 0;
        virtual triple::relation::TypesContents<T>* readRelationTypesContents(bool verbose = false) = 0;

        virtual triple::pattern::relation::BinaryMap<INT>* readBinaryPatterns(triple::pattern::Pattern pattern, triple::Encoder<T>* encoder, bool enableFilters = false, bool verbose = false) = 0;
        virtual triple::pattern::relation::UnarySet<INT>* readUnaryPatterns(triple::pattern::Pattern pattern, triple::Encoder<T>* encoder, bool enableFilters = false, bool verbose = false) = 0;
    };

}

#endif
