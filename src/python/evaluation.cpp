#include "../base/api/evaluation.h"

#include "utils/decode/metric.h"
#include "utils/encode/metric.h"

extern "C" void meager__python__api__evaluation__init(char* metrics, char* taskName, char* subsetName, bool verbose) {
    evaluation::task::Task task = meager::main::evaluation::task::decodeName(taskName);
    subset::Type subset = subset::decodeName(subsetName);

    meager::main::api::evaluation::init([metrics](string label){
        auto decodedMetrics = meager::python::utils::decode::metric::metrics<unsigned short>(metrics);
        return new meager::main::evaluation::metric::tracker::Set(decodedMetrics->items, decodedMetrics->length, label);
    }, task, subset, verbose);
}


extern "C" void* meager__python__api__evaluation__trial(char* componentName, bool verbose) {
    auto batch = meager::main::api::evaluation::trial(
        triple::decodeComponent(componentName),
        verbose
    );

    return meager::python::utils::encode::tripleBatch(batch, false);
}

extern "C" void meager__python__api__evaluation__evaluate(char* componentName, REAL* predictions, bool reverse, bool verbose) {
    meager::main::api::evaluation::evaluate(triple::decodeComponent(componentName), predictions, reverse, verbose);
}

extern "C" void* meager__python__api__evaluation__computeMetrics(bool verbose) {
    meager::main::evaluation::metric::tree::Root* root = meager::main::api::evaluation::computeMetrics(verbose);

    return meager::python::utils::encode::metric::tree<double, unsigned short>(root->tree, root->normalizationCoefficient);
}
