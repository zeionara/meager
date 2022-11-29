#ifndef PATTERNS_RELATION_MAP_H
#define PATTERNS_RELATION_MAP_H

#include <unordered_map>

using namespace std;

template <typename T>
struct RelationMap {
    unordered_map<T, T>** items;

    RelationMap() {}

    RelationMap(unordered_map<T, T>** items) {
        this->items = items;
    }
};

#endif
