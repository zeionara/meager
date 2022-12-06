#ifndef TRIPLE_RELATION_TYPE_H
#define TRIPLE_RELATION_TYPE_H

#include "main.h"
#include "TripleIndex.h"
#include "reader.h"

#define invalidArgument invalid_argument

template<typename T>
bool binarySearch(T* items, T target, long start, long end) {
  
  if (start > end) {
      return false;
  }
  
  long middle = (start + end) >> 1;
  // let middle = Math.floor((start + end) / 2)
  T middleItem = items[middle];
  if (middleItem == target) {
      return true;
  }
  
  if (target < middleItem) {
    return binarySearch(items, target, start, middle - 1);
  }

  return binarySearch(items, target, middle + 1, end);  // target > items[middle]
}

template <typename T>
struct AllowedTripleElements {
    INT* items;
    INT length;

    AllowedTripleElements(RelationTypeContents<T>* contents) {
        this->items = contents->entities;
        this->length = contents->length;

        std::sort(items, items + length);
    }

    bool contains(INT value) {  // TODO: implement binary search
        return binarySearch(items, value, 0, length);
        // T i = 0;
        // while (i < length && items[i] < value) i++;
        // cout << i << " | " << length << endl;
        // return i < length;
        // for (INT i = 0; i < length; i++) {
        //     if (items[i] == value) {
        //         return true;
        //     }
        // }
        // return false;
    }

    void encode(TripleComponentEncoder<INT>* encoder) {
        INT j = 0;
        for (INT i = 0; i < length; i++) {
            if (encoder->contains(items[i])) {
                items[j++] = encoder->encode(items[i]);
            }
        }
        length = j;

        std::sort(items, items + length);
    }
};

template <typename T>
struct RelationType {
    AllowedTripleElements<T>* heads;
    AllowedTripleElements<T>* tails;
    INT relation;

    RelationType(RelationTypeContents<T>* headContents, RelationTypeContents<T>* tailContents) {

        heads = new AllowedTripleElements<T>(headContents);

        if (headContents->relation != tailContents->relation) {
            throw invalidArgument("Relation type can be created only for one relation");
        }

        tails = new AllowedTripleElements<T>(tailContents);

        this->relation = headContents->relation;
    }

    void encode(TripleEncoder<INT>* encoder) {
        relation = encoder->relation->encode(relation);
        heads->encode(encoder->entity);
        tails->encode(encoder->entity);
    }
};

template <typename T>
struct RelationTypes {
    RelationType<T>** relations;
    INT length;

    RelationTypes(bool enableFilters, TripleEncoder<T>* encoder, CorpusReader<T>* reader, bool verbose = false) {
        RelationTypesContents<T>* contents = reader->readRelationTypesContents(verbose);

        relations = (RelationType<T>**)calloc(contents->length, sizeof(RelationType<T>*));

        this->length = contents->length;

        INT j = 0;
        for (INT i = 0; i < contents->length; i += 2) {
            RelationTypeContents<T>* head = contents->relations[i];

            if (!enableFilters || encoder->relation->contains(head->relation)) {
                RelationTypeContents<T>* tail = contents->relations[i + 1];

                if (enableFilters) {
                    head->encode(encoder);
                    tail->encode(encoder);
                }

                relations[j++] = new RelationType<T>(head, tail);
            }
        }
        length = j;
    }

    RelationType<T>* get(INT relation) {
        for (INT i = 0; i < this->length; i++) {
            if (this->relations[i]->relation == relation) {
                return this->relations[i];
            }
        }
        return nullptr;
    }
};

#endif
