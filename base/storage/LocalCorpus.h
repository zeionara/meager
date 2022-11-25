#ifndef STORAGE_LOCAL_CORPUS_H
#define STORAGE_LOCAL_CORPUS_H

#include "../triple/main.h"
#include "Corpus.h"
#include "CorpusReader.h"

using namespace std;

template <typename T>
struct LocalCorpus: Corpus {
    CorpusReader<T>* reader;
    bool enableFilters;

    LocalCorpus(CorpusReader<T>* reader, bool enableFilters = false) {
        this->reader = reader;
        this->enableFilters = enableFilters;
    }

    virtual void importTrain(bool verbose = false) = 0;
    virtual void importTest(bool verbose = false) = 0;
    virtual void importValid(bool verbose = false) = 0;
    virtual void importTypes(bool verbose = false) = 0;
};

#endif
