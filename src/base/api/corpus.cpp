#include "../storage/LocalTsvCorpus.h"
#include "../storage/OpenKECorpusReader.h"

// Init

LocalTsvCorpus<INT>* corpus;

bool constexpr DROP_PATTERN_DUPLICATES = true;

void initCorpus(char *path, bool enableFilters, bool verbose) {
    corpus = new LocalTsvCorpus<INT>(new OpenKECorpusReader(path), enableFilters, verbose);
    if (verbose) {
        printf("corpus path is set to %s\n", path);
    }
}

// Read

void importFilter(bool dropDuplicates = true, bool verbose = false) {
    corpus->importFilter(dropDuplicates, verbose); 
}

void importPattern(bool verbose = false) {
    corpus->importPattern(verbose); 
}

void importTrain(bool dropPatternDuplicates = true, bool verbose = false) {
    corpus->importTrain(dropPatternDuplicates, verbose); 
}
// 
// void importTest(bool verbose = false) {
//     corpus->importTest(verbose); 
// }
// 
// void importValid(bool verbose = false) {
//     corpus->importValid(verbose); 
// }

void importTriples(SubsetType subset, bool verbose) {
    switch (subset) {
        case train:
            corpus->importTrain(DROP_PATTERN_DUPLICATES, verbose); 
            return;
        case test:
            corpus->importTest(verbose); 
            return;
        case valid:
            corpus->importValid(verbose); 
            return;
        default:
            throw invalidArgument("Unsupported subset type given. Cannot import triples");
    }
}

void importTypes(bool verbose = false) {
    corpus->importTypes(verbose); 
}

// Get

long countEntities(bool verbose) {
    return corpus->countEntities();
}

long countRelations(bool verbose) {
    return corpus->countRelations();
}

long countTriples(SubsetType subset, bool verbose) {
    switch (subset) {
        case train:
            if (corpus->train == nullptr) {
                throw invalidArgument(TRAIN_IS_NOT_INITIALIZED);
            }
            return corpus->train->length;
        case test:
            if (corpus->test == nullptr) {
                throw invalidArgument(TEST_IS_NOT_INITIALIZED);
            }
            return corpus->test->length;
        case valid:
            if (corpus->valid == nullptr) {
                throw invalidArgument(VALID_IS_NOT_INITIALIZED);
            }
            return corpus->valid->length;
        default:
            throw invalidArgument("Unsupported subset type given. Cannot count triples");
    }
}

long countTriples(bool verbose) {
    // return countTrainTriples() + countTestTriples() + countValidTriples();
    return corpus->getLength();
}
