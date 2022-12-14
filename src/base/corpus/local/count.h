#ifndef CORPUS_LOCAL_COUNT_H
#define CORPUS_LOCAL_COUNT_H

#define invalidArgument invalid_argument

#include "../../types.h"
#include <stdexcept>

#include "constants.h"

namespace meager::main::corpus::local {

    template <typename T>
    struct Default;

    template <typename T>
    INT countEntities_(Default<T>* self) {
        if (self->enableFilters) {
            if (self->encoder == nullptr) {
                throw invalidArgument(ENCODER_IS_NOT_INITIALIZED);
            }
            return self->encoder->entity->nEncodedValues;
        }
        if (self->train == nullptr) {
            throw invalidArgument(TRAIN_IS_NOT_INITIALIZED);
        }
        return self->train->frequencies->nEntities;
    }

    template <typename T>
    INT countRelations_(Default<T>* self) {
        if (self->enableFilters) {
            if (self->encoder == nullptr) {
                throw invalidArgument(ENCODER_IS_NOT_INITIALIZED);
            }
            return self->encoder->relation->nEncodedValues;
        }
        if (self->train == nullptr) {
            throw invalidArgument(TRAIN_IS_NOT_INITIALIZED);
        }
        return self->train->frequencies->nRelations;
    }

    template <typename T>
    INT getLength_(Default<T>* self) {
        INT length = 0;

        if (self->train != nullptr) {
            length += self->train->length;
        }
        if (self->test != nullptr) {
            length += self->test->length;
        }
        if (self->valid != nullptr) {
            length += self->valid->length;
        }
        return length;
    }

}

#endif
