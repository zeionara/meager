#include "../base/api/corpus.h"

extern "C" void meager__python__api__corpus__init(char* path, bool enableFilters, bool verbose) {
    meager::main::api::corpus::init(path, enableFilters, verbose);
}

extern "C" void meager__python__api__corpus__countEntities(bool verbose) {
    meager::main::api::corpus::countEntities(verbose);
}

extern "C" void meager__python__api__corpus__countRelations(bool verbose) {
    meager::main::api::corpus::countRelations(verbose);
}

extern "C" void meager__python__api__corpus__countTriples_1(bool verbose) {
    meager::main::api::corpus::countTriples(verbose);
}

extern "C" void meager__python__api__corpus__countTriples_2(char* subsetName, bool verbose) {
    meager::main::api::corpus::countTriples(subset::decodeName(subsetName), verbose);
}

extern "C" void meager__python__api__corpus__importFilter(bool dropDuplicates, bool verbose) {
    meager::main::api::corpus::importFilter(dropDuplicates, verbose);
}

extern "C" void meager__python__api__corpus__importPattern(bool verbose) {
    meager::main::api::corpus::importPattern(verbose);
}

extern "C" void meager__python__api__corpus__importTrain(bool dropPatternDuplicates, bool verbose) {
    meager::main::api::corpus::importTrain(dropPatternDuplicates, verbose);
}

extern "C" void meager__python__api__corpus__importTriples(char* subsetName, bool verbose) {
    meager::main::api::corpus::importTriples(subset::decodeName(subsetName), verbose);
}

extern "C" void meager__python__api__corpus__importTypes(bool verbose) {
    meager::main::api::corpus::importTypes(verbose);
}
