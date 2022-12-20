#include "../base/sampling/batch/Triple.h"
#include "utils/encode/metric.h"

extern "C" void meager__python__api__delete__triple__batch(meager::main::sampling::batch::Triple* pointer, bool verbose) {
    delete pointer;
}

extern "C" void meager__python__api__delete__metree(meager::python::utils::encode::metric::EncodedTree* pointer, bool verbose) {
    delete pointer;
}
