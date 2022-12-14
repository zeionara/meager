#ifndef CORPUS_LOCAL_IMPORT_H
#define CORPUS_LOCAL_IMPORT_H

#define invalidArgument invalid_argument

#include <iostream>

#include "../../triple/list/ThickWrapper.h"
#include "../../triple/list/ThinWrapper.h"
#include "../../triple/relation/Types.h"

#include "constants.h"

using namespace std;

namespace meager::main::corpus::local {

    template <typename T>
    struct Default;

    template <typename T>
    void importPattern_(Default<T>* self, bool verbose) {
        if (verbose) {
            cout << "started reading pattern description templates" << endl;
        }

        self->patterns = new triple::pattern::description::Templates<T>(self->reader, self->encoder, self->enableFilters, verbose);

        if (verbose) {
            cout << "finished reading pattern description templates" << endl;
        }
    }

    template <typename T>
    void importTrain_(Default<T>* self, bool dropPatternDuplicates, bool verbose) {
        if (verbose) {
            cout << "started reading train subset" << endl;
        }

        if (self->train != nullptr) {
            throw invalidArgument(TRAIN_IS_ALREADY_INITIALIZED);
        }

        self->train = new triple::list::ThickWrapper<T>(subset::Type::train, self->reader, self->filter, self->encoder, self->patterns, self->enableFilters, dropPatternDuplicates, verbose);

        if (verbose) {
            cout << "finished reading train subset" << endl;
        }
    }

    template <typename T>
    void importTest_(Default<T>* self, bool verbose) {
        if (verbose) {
            cout << "started reading test subset" << endl;
        }

        if (self->test != nullptr) {
            throw invalidArgument(TEST_IS_ALREADY_INITIALIZED);
        }

        self->test = new triple::list::ThinWrapper<T>(subset::Type::test, self->reader, self->filter, self->encoder, self->enableFilters, verbose);

        if (verbose) {
            cout << "finished reading test subset" << endl;
        }
    }

    template <typename T>
    void importValid_(Default<T>* self, bool verbose) {
        if (verbose) {
            cout << "started reading valid subset" << endl;
        }

        if (self->valid != nullptr) {
            throw invalidArgument(VALID_IS_ALREADY_INITIALIZED);
        }

        self->valid = new triple::list::ThinWrapper<T>(subset::Type::valid, self->reader, self->filter, self->encoder, self->enableFilters, verbose);

        if (verbose) {
            cout << "finished reading valid subset" << endl;
        }
    }

    template <typename T>
    void importTypes_(Default<T>* self, bool verbose) {
        if (verbose) {
            cout << "started reading relation types" << endl;
        }

        self->types = new triple::relation::Types<T>(self->enableFilters, self->encoder, self->reader, verbose);

        if (verbose) {
            cout << "finished reading " << self->types->length << " relation types" << endl;
        }
    }

    template <typename T>
    void importFilter_(Default<T>* self, bool dropDuplicates, bool verbose) {
        if (self->enableFilters) {
            self->filter = new triple::filter::Filter<T>(self->reader, dropDuplicates, verbose);
        } else {
            if (verbose) {
                cout << "skip reading filter because filters are disabled" << endl;
            }
        }
    }

}

#endif
