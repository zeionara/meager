#ifndef CORPUS_LOCAL_CORPUS_H
#define CORPUS_LOCAL_CORPUS_H

// #include "../../triple/main.h"
#include "../Corpus.h"
#include "../reader/Reader.h"

using namespace std;
// using namespace meager::main::storage;

namespace meager::main::corpus::local {

    template <typename T>
    struct Corpus: corpus::Corpus {
        reader::Corpus<T>* reader;
        bool enableFilters;

        Corpus(reader::Corpus<T>* reader, bool enableFilters = false) {
            this->reader = reader;
            this->enableFilters = enableFilters;
        }

        virtual void importTrain(bool dropPatternDuplicates = true, bool verbose = false) = 0;
        virtual void importTest(bool verbose = false) = 0;
        virtual void importValid(bool verbose = false) = 0;
        virtual void importTypes(bool verbose = false) = 0;
    };

}

#endif
