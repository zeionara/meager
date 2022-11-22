#ifndef STORAGE_CORPUS_H
#define STORAGE_CORPUS_H

#include "../triple/main.h"

struct Corpus {
    virtual bool contains(Triple triple) = 0;
    virtual bool allows(Triple triple) = 0;
};

#endif
