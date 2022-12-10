#ifndef TRIPLE_TRIPLE_ENCODER_H
#define TRIPLE_TRIPLE_ENCODER_H

#include "TripleComponentEncoder.h"

using namespace std;

namespace meager::main::triple {

    template <typename T>
    struct Encoder {
        ComponentEncoder<T>* entity = new ComponentEncoder<T>();
        ComponentEncoder<T>* relation = new ComponentEncoder<T>();
    };

}

#endif
