#ifndef STORAGE_LOCAL_TSV_CORPUS_H
#define STORAGE_LOCAL_TSV_CORPUS_H

#include "../triple/main.h"
#include "LocalCorpus.h"
#include "../triple/list/ThickTripleListWrapper.h"

using namespace std;

struct LocalTsvCorpus: LocalCorpus {
    ThickTripleListWrapper* train;

    LocalTsvCorpus(string path): LocalCorpus(path) {};

    void importTrain(string path, bool enableFilters, bool verbose) {
        // cout << "started reading train subset for corpus from " << this->path + "/" + path << endl;
        cout << "started reading train subset for corpus from " << this->path + path << endl;
        // ThickTripleListWrapper* tmp = new ThickTripleListWrapper(::SubsetType::train, false, false);
        // cout << enableFilters << endl;
        // cout << verbose << endl;
        this->train = new ThickTripleListWrapper(this->path + path, enableFilters, verbose);
        // ThickTripleListWrapper* tmp = new ThickTripleListWrapper(::SubsetType::train, false, false);
        cout << "finished reading train subset for corpus" << endl;
    }

    bool contains(Triple triple) {
        return false;
    }
    bool allows(Triple triple) {
        return false;
    }
};

#endif
