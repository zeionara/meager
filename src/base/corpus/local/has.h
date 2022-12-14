#ifndef CORPUS_LOCAL_HAS_H
#define CORPUS_LOCAL_HAS_H

#include "../../triple/Triple.h"

namespace meager::main::corpus::local {

    template <typename T>
    struct Default;

    template <typename T>
    bool contains_(Default<T>* self, triple::Triple triple) {
        return (
            self->train != nullptr &&
            self->train->index->contains(triple)
        ) || (
            self->test != nullptr &&
            self->test->index->contains(triple)
        ) || (
            self->valid != nullptr &&
            self->valid->index->contains(triple)
        );
    }

    template <typename T>
    bool allowsHead(Default<T>* self, triple::Triple triple) {
        return self->types->get(triple.r)->heads->contains(triple.h);
    }

    template <typename T>
    bool allowsTail(Default<T>* self, triple::Triple triple) {
        return self->types->get(triple.r)->tails->contains(triple.t);
    }

    template <typename T>
    bool allows_(Default<T>* self, triple::Triple triple) {
        return allowsHead(self, triple) && allowsTail(self, triple);
    }

}

#endif
