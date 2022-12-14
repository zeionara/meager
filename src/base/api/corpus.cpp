#include "../corpus/local/Default.h"
#include "../corpus/reader/openke/Reader.h"

using namespace meager::main::utils;
using namespace meager::main::corpus;

// Init

local::Default<INT>* corpus;

bool constexpr DROP_PATTERN_DUPLICATES = true;

void initCorpus(char *path, bool enableFilters, bool verbose) {
    ::corpus = new local::Default<INT>(new reader::openke::Corpus(path), enableFilters, verbose);
    if (verbose) {
        printf("corpus path is set to %s\n", path);
    }
}

// Read

void importFilter(bool dropDuplicates = true, bool verbose = false) {
    ::corpus->importFilter(dropDuplicates, verbose); 
}

void importPattern(bool verbose = false) {
    ::corpus->importPattern(verbose); 
}

void importTrain(bool dropPatternDuplicates = true, bool verbose = false) {
    ::corpus->importTrain(dropPatternDuplicates, verbose); 
}
// 
// void importTest(bool verbose = false) {
//     corpus->importTest(verbose); 
// }
// 
// void importValid(bool verbose = false) {
//     corpus->importValid(verbose); 
// }

void importTriples(subset::Type subset, bool verbose) {
    switch (subset) {
        case subset::Type::train:
            ::corpus->importTrain(DROP_PATTERN_DUPLICATES, verbose); 
            return;
        case subset::Type::test:
            ::corpus->importTest(verbose); 
            return;
        case subset::Type::valid:
            ::corpus->importValid(verbose); 
            return;
        default:
            throw invalidArgument("Unsupported subset type given. Cannot import triples");
    }
}

void importTypes(bool verbose = false) {
    ::corpus->importTypes(verbose); 
}

// Get

long countEntities(bool verbose) {
    return ::corpus->countEntities();
}

long countRelations(bool verbose) {
    return ::corpus->countRelations();
}

long countTriples(subset::Type subset, bool verbose) {
    switch (subset) {
        case subset::Type::train:
            if (::corpus->train == nullptr) {
                throw invalidArgument(corpus::local::TRAIN_IS_NOT_INITIALIZED);
            }
            return ::corpus->train->length;
        case subset::Type::test:
            if (::corpus->test == nullptr) {
                throw invalidArgument(corpus::local::TEST_IS_NOT_INITIALIZED);
            }
            return ::corpus->test->length;
        case subset::Type::valid:
            if (::corpus->valid == nullptr) {
                throw invalidArgument(corpus::local::VALID_IS_NOT_INITIALIZED);
            }
            return ::corpus->valid->length;
        default:
            throw invalidArgument("Unsupported subset type given. Cannot count triples");
    }
}

long countTriples(bool verbose) {
    // return countTrainTriples() + countTestTriples() + countValidTriples();
    return ::corpus->getLength();
}
