#ifndef PATTERNS_UNARY_PATTERN_RELATION_SET_H
#define PATTERNS_UNARY_PATTERN_RELATION_SET_H

#include <unordered_set>

#include "PatternRelations.h"

template <typename T>
struct UnaryPatternRelationSet: PatternRelations {
    unordered_set<T> relations;

    UnaryPatternRelationSet(unordered_set<T> relations) {
        this->relations = relations;
    }
};

#endif
