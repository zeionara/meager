#include "../storage/LocalTsvCorpus.h"
#include "../storage/OpenKECorpusReader.h"

LocalTsvCorpus<long>* corpus;

void initCorpus(char *path, bool enableFilters, bool verbose) {
    // corpus = new LocalTsvCorpus<INT>(new OpenKECorpusReader(path), false);
    corpus = new LocalTsvCorpus<INT>(new OpenKECorpusReader(path), enableFilters, verbose);
    if (verbose) {
        printf("corpus path : %s\n", path);
    }
}

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

void importTrain(bool verbose = false) {
    corpus->importTrain(verbose); 
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

void importFilter(bool verbose = false, bool dropDuplicates = true) {
    corpus->importFilter(verbose, dropDuplicates); 
}
