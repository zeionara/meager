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

// ERL_NIF_TERM encodeMetric(ErlNifEnv* env, main::evaluation::metric::tracker::TrackerBase* metric) {
//     switch (metric->getId()) {
//         case main::evaluation::metric::Metric::Count:
//             return enif_make_tuple2(env, enif_make_atom(env, metric->getName().c_str()), enif_make_long(env, ((main::evaluation::metric::tracker::Count*)metric)->n));
//         default:
//             return enif_make_atom(env, metric->getName().c_str());
//     }
// }

// void** encodeMetricTree(meager::main::evaluation::metric::tree::Tree* tree, INT normalizationCoefficient, char* label) {
//     if (tree->length > 127) {
//         throw invalidArgument("Cannot encode node with more than 127 descendants");
//     }
// 
//     if (tree->nodes != nullptr) {
// 
//         // const char _length = 1;
// 
//         unsigned char* result = new unsigned char[sizeof(void*) * tree->length + sizeof(char*) + 1];
// 
//         result[0] = tree->length;
// 
//         void** nodes = (void**)(result + 1);
// 
//         // nodes[0] = result;
// 
//         char** name_pointer = (char**)(result + 1 + sizeof(void*) * tree->length);
// 
//         name_pointer[0] = label;
// 
//         // cout << tree->length << endl;
//         // ERL_NIF_TERM* encodedChildren = new ERL_NIF_TERM[tree->length]();
// 
//         for (INT i = 0; i < tree->length; i++) {
//             string label = tree->nodes[i]->label;
//             // cout << "label = " << label << endl;
//             main::evaluation::metric::tree::Tree* subtree = tree->nodes[i]->value;
// 
//             ERL_NIF_TERM encodedChild = enif_make_tuple2(env, enif_make_atom(env, label.c_str()), encodeMetricTree(env, subtree, normalizationCoefficient));
//             encodedChildren[i] = encodedChild;
//         }
// 
//         return enif_make_list_from_array(env, encodedChildren, tree->length);
//     } else {
//         ERL_NIF_TERM* encodedMetrics = new ERL_NIF_TERM[tree->metrics->length]();
// 
//         if (tree->metrics) {
//             for (INT i = 0; i < tree->metrics->length; i++) {
//                 main::evaluation::metric::tracker::TrackerBase* metric = tree->metrics->trackers[i];
// 
//                 // evaluation::metric::Metric id = metric->getId();
// 
//                 encodedMetrics[i] = enif_make_tuple2(env, encodeMetric(env, metric), enif_make_double(env, metric->divide(normalizationCoefficient)));
//             }
//         }
// 
// 
//         return enif_make_list_from_array(env, encodedMetrics, tree->metrics->length);
//     }
//     throw invalidArgument("Each metric tree node must either contain links to other nodes either contain a list of metrics");
// }


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

    //

    void** node = new void*[2];

    node[0] = result;
    node[1] = (char*)"filtered";

    void** node2 = new void*[2];

    node2[0] = result;
    node2[1] = (char*)"unfiltered";
    
    //

    const char _length = 2;

    unsigned char* _result = new unsigned char[sizeof(void*) * _length + 1];

    _result[0] = _length;

    void** nodes = (void**)(_result + 1);

    nodes[0] = node;
    nodes[1] = node2;

    //

    unsigned char** bar = new unsigned char*[1];
    // bar[0] = result;
    bar[0] = _result;

    return (void**)bar;
}
