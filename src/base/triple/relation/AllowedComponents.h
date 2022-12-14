#ifndef TRIPLE_RELATION_ALLOWED_COMPONENTS_H
#define TRIPLE_RELATION_ALLOWED_COMPONENTS_H

#include "../../types.h"

#include "../../corpus/reader/Reader.h"

using namespace std;

namespace meager::main::triple::relation {

    template<typename T>
    bool binarySearch(T* items, T target, long start, long end) {
      
      if (start > end) {
          return false;
      }
      
      long middle = (start + end) >> 1;

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
    struct AllowedComponents {
        INT* items;
        INT length;

        AllowedComponents(TypeContents<T>* contents, bool shouldSort = true) {
            this->items = contents->entities;
            this->length = contents->length;

            if (shouldSort) {
                sort(items, items + length);
            }
        }

        bool contains(INT value) {
            return binarySearch(items, value, 0, length);
        }

        void encode(ComponentEncoder<INT>* encoder) {
            INT j = 0;
            for (INT i = 0; i < length; i++) {
                if (encoder->contains(items[i])) {
                    items[j++] = encoder->encode(items[i]);
                }
            }
            length = j;

            sort(items, items + length);
        }
    };

}

#endif
