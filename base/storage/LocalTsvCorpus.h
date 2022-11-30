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

string const _TRAIN_FILENAME = "train2id.txt";
string const _TEST_FILENAME = "test2id.txt";
string const _VALID_FILENAME = "valid2id.txt";

string const _TYPE_FILENAME = "type_constrain.txt";

string const _ENTITIES_FILENAME = "entity2id.txt";
string const _RELATIONS_FILENAME = "relation2id.txt";

string const _INCLUDING_FILTERS_FILENAME = "filters/including.txt";
string const _EXCLUDING_FILTERS_FILENAME = "filters/excluding.txt";

template <typename T>
struct LocalTsvCorpus: LocalCorpus<T> {
    ThickTripleListWrapper<T>* train;

    ThinTripleListWrapper<T>* test;
    ThinTripleListWrapper<T>* valid;

    RelationTypes<T>* types;
    TripleFilter<T>* filter;
    TripleEncoder<T>* encoder;

    LocalTsvCorpus(CorpusReader<T>* reader, bool enableFilters = false, bool verbose = false): LocalCorpus<T>(reader, enableFilters) {
        if (enableFilters) {
            if (verbose) {
                cout << "filters are enabled" << endl;
            }
            encoder = new TripleEncoder<INT>();
        } else {
            encoder = nullptr;
            filter = nullptr;
        }
    };

    void importTrain(bool verbose) {
        cout << "started reading train subset for corpus from " << endl;
        this->train = new ThickTripleListWrapper<T>(::SubsetType::train, this->reader, filter, encoder, this->enableFilters, verbose);
        cout << "finished reading train subset for corpus" << endl;
    }

    void importTest(bool verbose) {
        cout << "started reading test subset for corpus from " << endl;
        this->test = new ThinTripleListWrapper<T>(::SubsetType::test, this->reader, filter, encoder, this->enableFilters, verbose);
        cout << "finished reading test subset for corpus" << endl;
    }

    void importValid(bool verbose) {
        this->valid = new ThinTripleListWrapper<T>(::SubsetType::valid, this->reader, filter, encoder, this->enableFilters, verbose);
    }

    void importTypes(bool verbose) {
        // cout << encoder->relation->contains(5) << endl;
        // cout << encoder->relation->contains(6) << endl;
        // cout << encoder->relation->contains(7) << endl;
        this->types = new RelationTypes<T>(this->enableFilters, encoder, this->reader, verbose);
        if (verbose) {
            cout << "Imported types for " << types->length << " relations" << endl;
        }
    }

    void importFilter(bool dropDuplicates, bool verbose) {
        if (this->enableFilters) {
            this->filter = new TripleFilter<T>(this->reader, dropDuplicates, verbose);
        } else {
            if (verbose) {
                cout << "skip reading filter because filters are disabled" << endl;
            }
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

    INT countEntities() {
        if (this->enableFilters) {
            return encoder->entity->nEncodedValues;
        }
        return train->frequencies->nEntities;
    }

    INT countRelations() {
        if (this->enableFilters) {
            return encoder->relation->nEncodedValues;
        }
        return train->frequencies->nRelations;
    }

    INT getLength() {
        return train->length + test->length + valid->length;
    }
};

#endif
