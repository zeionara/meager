#include "../base/api/corpus.h"

// namespace meager::python::api::corpus {

extern "C" void meager__python__api__corpus__init(char *path, bool enableFilters = false, bool verbose = false) {
    meager::main::api::corpus::init(path, enableFilters, verbose);
}

// }
