#include "../base/api/evaluation.h"
// #include "../base/evaluation/metric/main.h"
// 
// #include "../base/evaluation/metric/tracker/Count.h"
// #include "../base/evaluation/metric/tracker/Rank.h"
// #include "../base/evaluation/metric/tracker/ReciprocalRank.h"
#include "utils.h"

extern "C" void meager__python__api__evaluation__init(char* metrics, char* taskName, char* subsetName, bool verbose) {
    evaluation::task::Task task = meager::main::evaluation::task::decodeName(taskName);
    subset::Type subset = subset::decodeName(subsetName);

    meager::main::api::evaluation::init([metrics](string label){
        auto decodedMetrics = meager::python::utils::decode::metrics<unsigned short>(metrics);
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
    // printf("address = %d", predictions);
    // cout << endl;
    // printf("first prediction = %f", predictions[0]);
    // cout << endl;
    meager::main::api::evaluation::evaluate(triple::decodeComponent(componentName), predictions, reverse, verbose);
}
