#ifndef TRIPLE_RELATION_TYPE_H
#define TRIPLE_RELATION_TYPE_H

#include "AllowedComponents.h"

namespace meager::main::triple::relation {

    template <typename T>
    struct Type {
        AllowedComponents<T>* heads;
        AllowedComponents<T>* tails;

        INT relation;

        Type(TypeContents<T>* headContents, TypeContents<T>* tailContents, bool shouldSort = true) {

            heads = new AllowedComponents<T>(headContents, shouldSort);

            if (headContents->relation != tailContents->relation) {
                throw invalidArgument("Allowed components for head and tail must have the same relation");
            }

            tails = new AllowedComponents<T>(tailContents, shouldSort);

            this->relation = headContents->relation;
        }

        void encode(Encoder<INT>* encoder) {
            relation = encoder->relation->encode(relation);
            heads->encode(encoder->entity);
            tails->encode(encoder->entity);
        }
    };

}

#endif