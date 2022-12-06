#ifndef TRIPLE_TRIPLE_ENCODER_H
#define TRIPLE_TRIPLE_ENCODER_H

#include <unordered_map>
#include <vector>

#include "TripleComponentEncoder.h"
#include "main.h"

using namespace std;

template <typename T>
struct TripleEncoder {

    TripleComponentEncoder<T>* entity = new TripleComponentEncoder<T>();
    TripleComponentEncoder<T>* relation = new TripleComponentEncoder<T>();

    // Triple encode(Triple triple) {
    //     return Triple(entity->encode(triple.h), relation->encode(triple.r), entity->encode(triple.t));
    // }
};

#endif
