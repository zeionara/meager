#ifndef TRIPLE_TRIPLE_ENCODER_H
#define TRIPLE_TRIPLE_ENCODER_H

#include <unordered_map>
#include <vector>

#include "TripleComponentEncoder.h"
#include "main.h"

using namespace std;

struct TripleEncoder {

    TripleComponentEncoder* entity = new TripleComponentEncoder();
    TripleComponentEncoder* relation = new TripleComponentEncoder();

    Triple encode(Triple triple) {
        return Triple(entity->encode(triple.h), relation->encode(triple.r), entity->encode(triple.t));
    }
};

#endif
