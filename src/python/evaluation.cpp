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

char* string_to_heap_char(string str) {
    char* chars = new char[str.length()];
    strcpy(chars, str.c_str());
    return chars;
}

template <typename T>
void* encodeMetric(string name, T* parameters = NULL, unsigned char length = 0) {
    unsigned char* result = new unsigned char[1 + name.length() + 1 + sizeof(T) * length];

    result[0] = length;

    char* name_ = (char*)(result + 1);

    strcpy(name_, name.c_str());

    T* params = (T*)(result + 1 + name.length() + 1);

    for (unsigned char i = 0; i < length; i++) {
        params[i] = parameters[i];
    }

    return result;
}


void* encodeMetric(meager::main::evaluation::metric::tracker::TrackerBase* metric) {
    switch (metric->getId()) {
        case meager::main::evaluation::metric::Metric::Count: {
            unsigned short parameters[1] = {(unsigned short)(((meager::main::evaluation::metric::tracker::Count*)metric)->n)};
            return encodeMetric(metric->getName(), parameters, 1);
            break;
        }
        default:
            return encodeMetric<unsigned short>(metric->getName());
    }
}

void* encodeMetricTree(meager::main::evaluation::metric::tree::Tree* tree, INT normalizationCoefficient) {
    if (tree->length > 127) {
        throw invalidArgument("Cannot encode node with more than 127 descendants");
    }

    if (tree->nodes != nullptr) {

        unsigned char* result = new unsigned char[sizeof(void*) * tree->length + 1];

        result[0] = tree->length;

        void** nodes = (void**)(result + 1);

        for (INT i = 0; i < tree->length; i++) {
            string label = tree->nodes[i]->label;

            void** node = new void*[2];

            // node[0] = result;
            // node[1] = (void*)label.c_str();
            // char* name = new char[label.length()];
            // strcpy(name, label.c_str());

            node[1] = string_to_heap_char(label);
            // cout << "label = " << label << endl;
            auto subtree = tree->nodes[i]->value;
            node[0] = encodeMetricTree(subtree, normalizationCoefficient);

            nodes[i] = node;
        }

        return result;
    } else {

        if (tree->metrics) {

            // cout << "allocate memory for metrics" << sizeof(METRIC_T*) * tree->metrics->length + sizeof(char*) * tree->metrics->length + 1 << endl;
            unsigned char* result = new unsigned char[sizeof(METRIC_T*) * tree->metrics->length + sizeof(char*) * tree->metrics->length + 1];
            // cout << "allocated memory for metrics" << endl;

            result[0] = tree->metrics->length;
            result[0] |= (unsigned char)0x80;

            METRIC_T* values = (METRIC_T*)(result + 1);
            char** names = (char**)(result + 1 + sizeof(METRIC_T) * tree->metrics->length);

            for (INT i = 0; i < tree->metrics->length; i++) {
                auto metric = tree->metrics->trackers[i];

                values[i] = metric->divide(normalizationCoefficient);
                // names[i] = string_to_heap_char(encodeMetric(metric)); // (char*)encodeMetric(metric).c_str();
                names[i] = (char*)encodeMetric(metric); // (char*)encodeMetric(metric).c_str();
            }

            return result;
        }

        throw invalidArgument("If list of nodes is not set then metrics must be set");

    }
    throw invalidArgument("Each metric tree node must either contain links to other nodes either contain a list of metrics");
}


extern "C" void** meager__python__api__evaluation__computeMetrics(bool verbose) {
    meager::main::evaluation::metric::tree::Root* root = meager::main::api::evaluation::computeMetrics(verbose);

    unsigned char** baz = new unsigned char*[1];
    // bar[0] = result;
    // cout << "Encoding metrics tree" << endl;
    baz[0] = (unsigned char*)encodeMetricTree(root->tree, root->normalizationCoefficient);

    return (void**)baz;

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

    // names[0] = (char*)"foo";
    // names[1] = (char*)"bar";
    // names[2] = (char*)"qux";
    
    unsigned short params[2] = {17, 19};

    names[0] = (char*)encodeMetric("foo", params, 2);
    names[1] = (char*)encodeMetric("bar", params, 2);
    names[2] = (char*)encodeMetric("baz", params, 2);

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
