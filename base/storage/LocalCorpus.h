#ifndef STORAGE_LOCAL_CORPUS_H
#define STORAGE_LOCAL_CORPUS_H

#include "../triple/main.h"
#include "Corpus.h"

using namespace std;

struct LocalCorpus: Corpus {
    string path;
    bool enableFilters;

    LocalCorpus(string path, bool enableFilters = false) {
        this->path = path;
        this->enableFilters = enableFilters;
    }

    virtual void importTrain(bool verbose = false) = 0;
    virtual void importTest(bool verbose = false) = 0;
    virtual void importValid(bool verbose = false) = 0;
    virtual void importTypes(bool verbose = false) = 0;
};

#endif
