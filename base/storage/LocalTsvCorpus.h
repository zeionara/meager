#ifndef STORAGE_LOCAL_TSV_CORPUS_H
#define STORAGE_LOCAL_TSV_CORPUS_H

#include "../triple/main.h"
#include "../triple/type.h"
#include "LocalCorpus.h"
#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"

using namespace std;

string const TRAIN_FILENAME = "train2id.txt";
string const TEST_FILENAME = "test2id.txt";
string const VALID_FILENAME = "valid2id.txt";

string const TYPE_FILENAME = "type_constrain.txt";

string const ENTITIES_FILENAME = "entity2id.txt";
string const RELATIONS_FILENAME = "relation2id.txt";

struct LocalTsvCorpus: LocalCorpus {
    ThickTripleListWrapper* train;

    ThinTripleListWrapper* test;
    ThinTripleListWrapper* valid;

    RelationTypes* types;

    LocalTsvCorpus(string path, bool enableFilters = false): LocalCorpus(path, enableFilters) {};

    void importTrain(bool verbose) {
        // cout << "started reading train subset for corpus from " << this->path + path << endl;
        this->train = new ThickTripleListWrapper(path + TRAIN_FILENAME, enableFilters, verbose);
        // cout << "finished reading train subset for corpus" << endl;
    }

    void importTest(bool verbose) {
        this->test = new ThinTripleListWrapper(path + TEST_FILENAME, enableFilters, verbose);
    }

    void importValid(bool verbose) {
        this->valid = new ThinTripleListWrapper(path + VALID_FILENAME, enableFilters, verbose);
    }

    void importTypes(bool verbose) {
        this->types = new RelationTypes(path + TYPE_FILENAME, verbose);
    }

    bool contains(Triple triple) {
        return false;
    }
    bool allows(Triple triple) {
        return false;
    }
};

#endif
