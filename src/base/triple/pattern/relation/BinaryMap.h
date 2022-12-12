#ifndef TRIPLE_PATTERN_RELATION_BINARY_MAP_H
#define TRIPLE_PATTERN_RELATION_BINARY_MAP_H

#include <unordered_map>

#include "Relations.h"

using namespace std;

namespace meager::main::triple::pattern::relation {

    template <typename T>
    struct BinaryMap: Relations {
        unordered_map<T, T> firstToSecond;
        unordered_map<T, T> secondToFirst;

        BinaryMap(unordered_map<T, T> firstToSecond, unordered_map<T, T> secondToFirst) {
            this->firstToSecond = firstToSecond;
            this->secondToFirst = secondToFirst;
        }
    };

}

#endif
