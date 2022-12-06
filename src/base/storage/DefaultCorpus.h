#ifndef STORAGE_DEFAULT_CORPUS_H
#define STORAGE_DEFAULT_CORPUS_H

#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
#include "../triple/type.h"

#include "Corpus.h"

struct DefaultCorpus: Corpus {

    ThickTripleListWrapper<INT>* train;
    ThinTripleListWrapper<INT>* test;
    ThinTripleListWrapper<INT>* valid;

    RelationTypes<INT>* types;

    DefaultCorpus(ThickTripleListWrapper<INT>* train, ThinTripleListWrapper<INT>* test, ThinTripleListWrapper<INT>* valid, RelationTypes<INT>* types) {
        this->train = train;
        this->test = test;
        this->valid = valid;

        this->types = types;
    }

    bool contains(Triple triple) {
        return train->index->contains(triple) || test->index->contains(triple) || valid->index->contains(triple);
    }

    bool allowsHead(Triple triple) {
        return types->get(triple.r)->heads->contains(triple.h);
    }

    bool allowsTail(Triple triple) {
        return types->get(triple.r)->tails->contains(triple.t);
    }

    bool allows(Triple triple) {
        return allowsHead(triple) && allowsTail(triple);
    }

};

#endif
