#ifndef STORAGE_CORPUS_H
#define STORAGE_CORPUS_H

// #include "../triple/main.h"
#include "../triple/Triple.h"

using namespace meager::main;

struct Corpus {
    virtual bool contains(triple::Triple triple) = 0;
    virtual bool allows(triple::Triple triple) = 0;
    virtual long countEntities() = 0;
    virtual long countRelations() = 0;
};

#endif
