#ifndef STORAGE_LOCAL_TSV_CORPUS_H
#define STORAGE_LOCAL_TSV_CORPUS_H

#include "../triple/main.h"
#include "../triple/type.h"
#include "LocalCorpus.h"
#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
#include "../filters/TripleFilter.h"
#include "../triple/TripleEncoder.h"
#include "../patterns/PatternDescriptionTemplates.h"

using namespace std;

string const _TRAIN_FILENAME = "train2id.txt";
string const _TEST_FILENAME = "test2id.txt";
string const _VALID_FILENAME = "valid2id.txt";

string const _TYPE_FILENAME = "type_constrain.txt";

string const _ENTITIES_FILENAME = "entity2id.txt";
string const _RELATIONS_FILENAME = "relation2id.txt";

string const _INCLUDING_FILTERS_FILENAME = "filters/including.txt";
string const _EXCLUDING_FILTERS_FILENAME = "filters/excluding.txt";

string const IS_NOT_INITIALIZED = "is not initialized";

string const ENCODER_IS_NOT_INITIALIZED = "Encoder " + IS_NOT_INITIALIZED;
string const TRAIN_IS_NOT_INITIALIZED = "Train subset " + IS_NOT_INITIALIZED;
string const TEST_IS_NOT_INITIALIZED = "Test subset " + IS_NOT_INITIALIZED;
string const VALID_IS_NOT_INITIALIZED = "Valid subset " + IS_NOT_INITIALIZED;

template <typename T>
struct LocalTsvCorpus: LocalCorpus<T> {
    ThickTripleListWrapper<T>* train;

    ThinTripleListWrapper<T>* test;
    ThinTripleListWrapper<T>* valid;

    RelationTypes<T>* types;
    TripleFilter<T>* filter;
    TripleEncoder<T>* encoder;
    PatternDescriptionTemplates<T>* patterns;

    LocalTsvCorpus(CorpusReader<T>* reader, bool enableFilters = false, bool verbose = false): LocalCorpus<T>(reader, enableFilters) {
        if (enableFilters) {
            if (verbose) {
                cout << "filters are enabled" << endl;
            }
            encoder = new TripleEncoder<INT>();
        } else {
            encoder = nullptr;
        }
        train = nullptr;
        test = nullptr;
        valid = nullptr;
        filter = nullptr;
        types = nullptr;
        patterns = nullptr;
    };

    void importPattern(bool verbose) {
        if (verbose) {
            cout << "started reading pattern description templates" << endl;
        }

        this->patterns = new PatternDescriptionTemplates<T>(this->reader, encoder, this->enableFilters, verbose);

        if (verbose) {
            cout << "finished reading pattern description templates" << endl;
        }
    }

    void importTrain(bool dropPatternDuplicates, bool verbose) {
        if (verbose) {
            cout << "started reading train subset" << endl;
        }

        this->train = new ThickTripleListWrapper<T>(::SubsetType::train, this->reader, filter, encoder, patterns, this->enableFilters, dropPatternDuplicates, verbose);

        if (verbose) {
            cout << "finished reading train subset" << endl;
        }
    }

    void importTest(bool verbose) {
        if (verbose) {
            cout << "started reading test subset" << endl;
        }

        this->test = new ThinTripleListWrapper<T>(::SubsetType::test, this->reader, filter, encoder, this->enableFilters, verbose);

        if (verbose) {
            cout << "finished reading test subset" << endl;
        }
    }

    void importValid(bool verbose) {
        if (verbose) {
            cout << "started reading valid subset" << endl;
        }

        this->valid = new ThinTripleListWrapper<T>(::SubsetType::valid, this->reader, filter, encoder, this->enableFilters, verbose);

        if (verbose) {
            cout << "finished reading valid subset" << endl;
        }
    }

    void importTypes(bool verbose) {
        if (verbose) {
            cout << "started reading relation types" << endl;
        }

        this->types = new RelationTypes<T>(this->enableFilters, encoder, this->reader, verbose);

        if (verbose) {
            cout << "finished reading " << types->length << " relation types" << endl;
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
        return (
            train != nullptr &&
            train->index->contains(triple)
        ) || (
            test != nullptr &&
            test->index->contains(triple)
        ) || (
            valid != nullptr &&
            valid->index->contains(triple)
        );
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
            if (encoder == nullptr) {
                throw invalidArgument(ENCODER_IS_NOT_INITIALIZED);
            }
            return encoder->entity->nEncodedValues;
        }
        if (train == nullptr) {
            throw invalidArgument(TRAIN_IS_NOT_INITIALIZED);
        }
        return train->frequencies->nEntities;
    }

    INT countRelations() {
        if (this->enableFilters) {
            if (encoder == nullptr) {
                throw invalidArgument(ENCODER_IS_NOT_INITIALIZED);
            }
            return encoder->relation->nEncodedValues;
        }
        if (train == nullptr) {
            throw invalidArgument(TRAIN_IS_NOT_INITIALIZED);
        }
        return train->frequencies->nRelations;
    }

    INT getLength() {
        INT length = 0;

        if (train != nullptr) {
            length += train->length;
        }
        if (test != nullptr) {
            length += test->length;
        }
        if (valid != nullptr) {
            length += valid->length;
        }
        return length;
    }
};

#endif
