#ifndef PATTERNS_BINARY_PATTERN_RELATION_MAP_H
#define PATTERNS_BINARY_PATTERN_RELATION_MAP_H

#include <unordered_map>

#include "PatternRelations.h"

template <typename T>
struct BinaryPatternRelationMap: PatternRelations {
    unordered_map<T, T> firstToSecond;
    unordered_map<T, T> secondToFirst;

    BinaryPatternRelationMap(unordered_map<T, T> firstToSecond, unordered_map<T, T> secondToFirst) {
        this->firstToSecond = firstToSecond;
        this->secondToFirst = secondToFirst;
    }
};

#endif
