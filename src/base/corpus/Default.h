#ifndef CORPUS_DEFAULT_H
#define CORPUS_DEFAULT_H

#include "../triple/list/ThickWrapper.h"
#include "../triple/list/ThinWrapper.h"

#include "../triple/relation/Types.h"

#include "Corpus.h"

namespace meager::main::corpus {

    struct Default: Corpus {

        triple::list::ThickWrapper<INT>* train;
        triple::list::ThinWrapper<INT>* test;
        triple::list::ThinWrapper<INT>* valid;

        triple::relation::Types<INT>* types;

        Default(triple::list::ThickWrapper<INT>* train, triple::list::ThinWrapper<INT>* test, triple::list::ThinWrapper<INT>* valid, triple::relation::Types<INT>* types) {
            this->train = train;
            this->test = test;
            this->valid = valid;

            this->types = types;
        }

        bool contains(triple::Triple triple) {
            return train->index->contains(triple) || test->index->contains(triple) || valid->index->contains(triple);
        }

        bool allowsHead(triple::Triple triple) {
            return types->get(triple.r)->heads->contains(triple.h);
        }

        bool allowsTail(triple::Triple triple) {
            return types->get(triple.r)->tails->contains(triple.t);
        }

        bool allows(triple::Triple triple) {
            return allowsHead(triple) && allowsTail(triple);
        }

    };

}

#endif