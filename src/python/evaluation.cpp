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

#define METRIC_T double

extern "C" void** meager__python__api__evaluation__computeMetrics(bool verbose) {
    meager::main::evaluation::metric::tree::Root* root = meager::main::api::evaluation::computeMetrics(verbose);

    // long* foo = new long[1];
    // foo[0] = 2;

    // long** bar = new long*[1];
    // bar[0] = foo;

    // // return root;
    // return (void**)bar;

    const char length = 3;

    unsigned char* result = new unsigned char[sizeof(METRIC_T) * length + sizeof(char*) * length + 1];

    result[0] = length;
    result[0] |= (unsigned char)0x80;

    METRIC_T* values = (METRIC_T*)(result + 1);

    values[0] = 0.1;
    values[1] = 0.2;
    values[2] = 0.3;
    
    char** names = (char**)(result + 1 + sizeof(METRIC_T) * length);

    names[0] = (char*)"foo";
    names[1] = (char*)"bar";
    names[2] = (char*)"qux";

    unsigned char** bar = new unsigned char*[1];
    bar[0] = result;

    return (void**)bar;
}
