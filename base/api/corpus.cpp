#include "../storage/LocalTsvCorpus.h"
#include "../storage/OpenKECorpusReader.h"

// Init

LocalTsvCorpus<INT>* corpus;

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

void importTest(bool verbose = false) {
    corpus->importTest(verbose); 
}

void importValid(bool verbose = false) {
    corpus->importValid(verbose); 
}

void importTypes(bool verbose = false) {
    corpus->importTypes(verbose); 
}

// Get

long countEntities() {
    return corpus->countEntities();
}

long countRelations() {
    return corpus->countRelations();
}

long countTrainTriples() {
    return corpus->train->length;
}

long countTestTriples() {
    return corpus->test->length;
}

long countValidTriples() {
    return corpus->valid->length;
}

long countTriples() {
    return countTrainTriples() + countTestTriples() + countValidTriples();
}
