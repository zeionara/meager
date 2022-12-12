#ifndef TRIPLE_PATTERN_RELATION_UNARY_SET_H
#define TRIPLE_PATTERN_RELATION_UNARY_SET_H

#include <unordered_set>

#include "Relations.h"

using namespace std;

namespace meager::main::triple::pattern::relation {

    template <typename T>
    struct UnarySet: Relations {
        unordered_set<T> relations;

        UnarySet(unordered_set<T> relations) {
            this->relations = relations;
        }
    };

}

#endif
