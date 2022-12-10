#ifndef STORAGE_DEFAULT_CORPUS_H
#define STORAGE_DEFAULT_CORPUS_H

#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
// #include "../triple/type.h"

#include "../triple/relation/RelationTypes.h"

#include "Corpus.h"

struct DefaultCorpus: Corpus {

    triple::ThickTripleListWrapper<INT>* train;
    triple::ThinTripleListWrapper<INT>* test;
    triple::ThinTripleListWrapper<INT>* valid;

    triple::relation::Types<INT>* types;

    DefaultCorpus(triple::ThickTripleListWrapper<INT>* train, triple::ThinTripleListWrapper<INT>* test, triple::ThinTripleListWrapper<INT>* valid, triple::relation::Types<INT>* types) {
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

#endif
