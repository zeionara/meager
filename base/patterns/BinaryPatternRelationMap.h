#ifndef PATTERNS_BINARY_PATTERN_RELATION_MAP_H
#define PATTERNS_BINARY_PATTERN_RELATION_MAP_H

#include "RelationMap.h"

template <typename T>
struct BinaryPatternRelationMap: RelationMap<T> {
    unordered_map<T, T> firstToSecond;
    unordered_map<T, T> secondToFirst;

    BinaryPatternRelationMap(unordered_map<T, T> firstToSecond, unordered_map<T, T> secondToFirst) {
        this->firstToSecond = firstToSecond;
        this->secondToFirst = secondToFirst;
        unordered_map<T, T>* items[2] = {&firstToSecond, &secondToFirst};
        this->items = items;
    }
};

#endif
