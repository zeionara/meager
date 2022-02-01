#include "../base/Reader.h"

extern "C"
void import_train_files(bool verbose = false, bool enable_filters = false) {
    importTrainFiles(verbose, enable_filters);
}

extern "C"
void import_test_files(bool verbose = false, bool enable_filters = false) {
    importTestFiles(verbose, enable_filters);
}

extern "C"
void import_type_files() {
    importTypeFiles();
}

