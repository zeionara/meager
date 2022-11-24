#ifndef STORAGE_LOCAL_TSV_CORPUS_H
#define STORAGE_LOCAL_TSV_CORPUS_H

#include "../triple/main.h"
#include "LocalCorpus.h"
#include "../triple/list/ThickTripleListWrapper.h"

using namespace std;

struct LocalTsvCorpus: LocalCorpus {
    ThickTripleListWrapper* train;

    LocalTsvCorpus(string path): LocalCorpus(path) {};

    void importTrain(bool enableFilters, bool verbose) {
        cout << "started reading train subset for corpus from " << this->path + path << endl;
        this->train = new ThickTripleListWrapper(path + getFilename(::SubsetType::train), enableFilters, verbose);
        cout << "finished reading train subset for corpus" << endl;
    }

    bool contains(Triple triple) {
        return false;
    }
    bool allows(Triple triple) {
        return false;
    }

    static string getFilename(SubsetType subsetType) {
        switch (subsetType) {
            case ::SubsetType::train:
                return "train2id.txt";
            case test:
                return "test2id.txt";
            case valid:
                return "valid2id.txt";
        }

        cout << "Incorrect subset type" << endl;
        throw "Incorrect subset type";
    }
};

#endif
