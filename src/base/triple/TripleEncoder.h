#ifndef TRIPLE_TRIPLE_ENCODER_H
#define TRIPLE_TRIPLE_ENCODER_H

#include "TripleComponentEncoder.h"

using namespace std;

namespace meager::main::triple {

    template <typename T>
    struct TripleEncoder {
        TripleComponentEncoder<T>* entity = new TripleComponentEncoder<T>();
        TripleComponentEncoder<T>* relation = new TripleComponentEncoder<T>();
    };

}

#endif
