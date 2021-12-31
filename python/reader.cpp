#include "../base/Reader.h"

extern "C"
void import_train_files(bool verbose = false) {
    importTrainFiles(verbose);
}

extern "C"
void import_test_files(bool verbose = false) {
    importTestFiles(verbose);
}

extern "C"
void import_type_files() {
    importTypeFiles();
}

