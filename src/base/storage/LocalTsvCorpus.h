#ifndef STORAGE_LOCAL_TSV_CORPUS_H
#define STORAGE_LOCAL_TSV_CORPUS_H

#include "../triple/main.h"
// #include "../triple/type.h"
#include "LocalCorpus.h"
#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
#include "../filters/TripleFilter.h"
#include "../triple/TripleEncoder.h"
#include "../patterns/PatternDescriptionTemplates.h"
#include "../evaluation/Stopwatch.h"

#include "../triple/relation/RelationTypes.h"

using namespace std;
using namespace meager::main;
using namespace meager::main::storage;

string const _TRAIN_FILENAME = "train2id.txt";
string const _TEST_FILENAME = "test2id.txt";
string const _VALID_FILENAME = "valid2id.txt";

string const _TYPE_FILENAME = "type_constrain.txt";

string const _ENTITIES_FILENAME = "entity2id.txt";
string const _RELATIONS_FILENAME = "relation2id.txt";

string const _INCLUDING_FILTERS_FILENAME = "filters/including.txt";
string const _EXCLUDING_FILTERS_FILENAME = "filters/excluding.txt";

string const IS_NOT_INITIALIZED = "is not initialized";
string const IS_ALREADY_INITIALIZED = "is already initialized";

string const ENCODER_IS_NOT_INITIALIZED = "Encoder " + IS_NOT_INITIALIZED;
string const TRAIN_IS_NOT_INITIALIZED = "Train subset " + IS_NOT_INITIALIZED;
string const TEST_IS_NOT_INITIALIZED = "Test subset " + IS_NOT_INITIALIZED;
string const VALID_IS_NOT_INITIALIZED = "Valid subset " + IS_NOT_INITIALIZED;

string const TRAIN_IS_ALREADY_INITIALIZED = "Train subset " + IS_ALREADY_INITIALIZED;
string const TEST_IS_ALREADY_INITIALIZED = "Test subset " + IS_ALREADY_INITIALIZED;
string const VALID_IS_ALREADY_INITIALIZED = "Valid subset " + IS_ALREADY_INITIALIZED;

template <typename T>
struct LocalTsvCorpus: LocalCorpus<T> {
    triple::ThickTripleListWrapper<T>* train;

    triple::ThinTripleListWrapper<T>* test;
    triple::ThinTripleListWrapper<T>* valid;

    triple::relation::Types<T>* types;
    TripleFilter<T>* filter;
    triple::Encoder<T>* encoder;
    PatternDescriptionTemplates<T>* patterns;

    LocalTsvCorpus(reader::Corpus<T>* reader, bool enableFilters = false, bool verbose = false): LocalCorpus<T>(reader, enableFilters) {
        if (enableFilters) {
            if (verbose) {
                cout << "filters are enabled" << endl;
            }
            encoder = new triple::Encoder<INT>();
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

        if (train != nullptr) {
            throw invalidArgument(TRAIN_IS_ALREADY_INITIALIZED);
        }

        this->train = new triple::ThickTripleListWrapper<T>(::SubsetType::train, this->reader, filter, encoder, patterns, this->enableFilters, dropPatternDuplicates, verbose);

        if (verbose) {
            cout << "finished reading train subset" << endl;
        }
    }

    void importTest(bool verbose) {
        if (verbose) {
            cout << "started reading test subset" << endl;
        }

        if (test != nullptr) {
            throw invalidArgument(TEST_IS_ALREADY_INITIALIZED);
        }

        this->test = new triple::ThinTripleListWrapper<T>(::SubsetType::test, this->reader, filter, encoder, this->enableFilters, verbose);

        if (verbose) {
            cout << "finished reading test subset" << endl;
        }
    }

    void importValid(bool verbose) {
        if (verbose) {
            cout << "started reading valid subset" << endl;
        }

        if (valid != nullptr) {
            throw invalidArgument(VALID_IS_ALREADY_INITIALIZED);
        }

        this->valid = new triple::ThinTripleListWrapper<T>(::SubsetType::valid, this->reader, filter, encoder, this->enableFilters, verbose);

        if (verbose) {
            cout << "finished reading valid subset" << endl;
        }
    }

    void importTypes(bool verbose) {
        if (verbose) {
            cout << "started reading relation types" << endl;
        }

        this->types = new triple::relation::Types<T>(this->enableFilters, encoder, this->reader, verbose);

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

    bool contains(triple::Triple triple) {
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

    bool allowsHead(triple::Triple triple) {
        // auto typeStopWatch = Stopwatch(1);
        // auto resultStopWatch = Stopwatch(1);

        // typeStopWatch.start();
        // auto type = types->get(triple.r);
        // typeStopWatch.stop();
        // resultStopWatch.start();
        // auto result = type->heads->contains(triple.h);
        // resultStopWatch.stop();

        // cout << endl << endl;
        // typeStopWatch.print("relation type search");
        // resultStopWatch.print("entity search");
        // cout << endl << endl;
        // return result;
        return types->get(triple.r)->heads->contains(triple.h);
    }

    bool allowsTail(triple::Triple triple) {
        return types->get(triple.r)->tails->contains(triple.t);
    }

    bool allows(triple::Triple triple) {
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
