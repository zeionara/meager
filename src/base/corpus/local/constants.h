#ifndef CORPUS_LOCAL_CONSTANTS_H
#define CORPUS_LOCAL_CONSTANTS_H

#include <string>

using namespace std;

namespace meager::main::corpus::local {

    string const IS_NOT_INITIALIZED = "is not initialized";
    string const IS_ALREADY_INITIALIZED = "is already initialized";

    string const ENCODER_IS_NOT_INITIALIZED = "Encoder " + IS_NOT_INITIALIZED;
    string const TRAIN_IS_NOT_INITIALIZED = "Train subset " + IS_NOT_INITIALIZED;
    string const TEST_IS_NOT_INITIALIZED = "Test subset " + IS_NOT_INITIALIZED;
    string const VALID_IS_NOT_INITIALIZED = "Valid subset " + IS_NOT_INITIALIZED;

    string const TRAIN_IS_ALREADY_INITIALIZED = "Train subset " + IS_ALREADY_INITIALIZED;
    string const TEST_IS_ALREADY_INITIALIZED = "Test subset " + IS_ALREADY_INITIALIZED;
    string const VALID_IS_ALREADY_INITIALIZED = "Valid subset " + IS_ALREADY_INITIALIZED;

}

#endif
