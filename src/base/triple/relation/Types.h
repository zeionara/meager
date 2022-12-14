#ifndef TRIPLE_RELATION_TYPES_H
#define TRIPLE_RELATION_TYPES_H

#include "Type.h"

// using namespace meager::main::storage;

namespace meager::main::triple::relation {

    template <typename T>
    struct Types {
        Type<T>** relations;
        INT length;

        Types(bool enableFilters, Encoder<T>* encoder, corpus::reader::Corpus<T>* reader, bool verbose = false) {
            TypesContents<T>* contents = reader->readRelationTypesContents(verbose);

            // relations = (Type<T>**)calloc(contents->length, sizeof(Type<T>*));
            relations = new Type<T>*[contents->length]; // (Type<T>**)calloc(contents->length, sizeof(Type<T>*));

            this->length = contents->length;

            INT j = 0;
            for (INT i = 0; i < contents->length; i += 2) {
                TypeContents<T>* head = contents->relations[i];

                if (!enableFilters || encoder->relation->contains(head->relation)) {
                    TypeContents<T>* tail = contents->relations[i + 1];

                    if (enableFilters) {
                        head->encode(encoder);
                        tail->encode(encoder);
                    }

                    relations[j++] = new Type<T>(head, tail);
                }
            }
            length = j;
        }

        Type<T>* get(INT relation) {
            for (INT i = 0; i < this->length; i++) {
                if (this->relations[i]->relation == relation) {
                    return this->relations[i];
                }
            }
            return nullptr;
        }
    };

}

#endif