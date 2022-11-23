#ifndef STORAGE_LOCAL_CORPUS_H
#define STORAGE_LOCAL_CORPUS_H

#include "../triple/main.h"
#include "Corpus.h"

using namespace std;

struct LocalCorpus: Corpus {
    string path;

    LocalCorpus(string path) {
        this->path = path;
    }

    virtual void importTrain(string path, bool enableFilters = false, bool verbose = false) = 0;
};

#endif
