#ifndef STORAGE_LOCAL_TSV_CORPUS_H
#define STORAGE_LOCAL_TSV_CORPUS_H

#include "../triple/main.h"
#include "../triple/type.h"
#include "LocalCorpus.h"
#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
#include "../filters/TripleFilter.h"
#include "../triple/TripleEncoder.h"

using namespace std;

string const TRAIN_FILENAME = "train2id.txt";
string const TEST_FILENAME = "test2id.txt";
string const VALID_FILENAME = "valid2id.txt";

string const TYPE_FILENAME = "type_constrain.txt";

string const ENTITIES_FILENAME = "entity2id.txt";
string const RELATIONS_FILENAME = "relation2id.txt";

string const INCLUDING_FILTERS_FILENAME = "filters/including.txt";
string const EXCLUDING_FILTERS_FILENAME = "filters/excluding.txt";

struct LocalTsvCorpus: LocalCorpus {
    ThickTripleListWrapper* train;

    ThinTripleListWrapper* test;
    ThinTripleListWrapper* valid;

    RelationTypes* types;
    TripleFilter* filter;
    TripleEncoder<INT>* encoder;

    LocalTsvCorpus(string path, bool enableFilters = false): LocalCorpus(path, enableFilters) {
        if (enableFilters) {
            cout << "FILTERS ARE ENABLED" << endl;
            encoder = new TripleEncoder<INT>();
        }
    };

    void importTrain(bool verbose) {
        cout << "started reading train subset for corpus from " << this->path + path << endl;
        this->train = new ThickTripleListWrapper(path + TRAIN_FILENAME, enableFilters, filter, encoder, verbose);
        cout << "finished reading train subset for corpus" << endl;
    }

    void importTest(bool verbose) {
        cout << "started reading test subset for corpus from " << this->path + path << endl;
        this->test = new ThinTripleListWrapper(path + TEST_FILENAME, enableFilters, filter, encoder, verbose);
        cout << "finished reading test subset for corpus" << endl;
    }

    void importValid(bool verbose) {
        this->valid = new ThinTripleListWrapper(path + VALID_FILENAME, enableFilters, filter, encoder, verbose);
    }

    void importTypes(bool verbose) {
        this->types = new RelationTypes(path + TYPE_FILENAME, enableFilters, encoder, verbose);
        if (verbose) {
            cout << "Imported types for " << types->length << " relations" << endl;
        }
    }

    void importFilter(bool verbose, bool dropDuplicates) {
        if (enableFilters) {
            this->filter = new TripleFilter(verbose, dropDuplicates, path + INCLUDING_FILTERS_FILENAME, path + EXCLUDING_FILTERS_FILENAME);
        }
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

    INT getLength() {
        return train->length + test->length + valid->length;
    }
};

#endif
