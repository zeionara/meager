// #include "../base/Setting.h"
#include "../base/api/corpus.h"

// using namespace meager::main::utils;
using namespace meager;

extern "C"
void set_in_path(char *path, bool as_tsv = false) {
    main::api::corpus::init(path);
}

extern "C"
void set_bern(INT con) {
    // setBern(con);
}

extern "C"
void set_work_threads(INT threads) {
    // setWorkThreads(threads);
}

extern "C"
INT get_relation_total() {
    return main::api::corpus::countRelations();
}

extern "C"
INT get_entity_total() {
    return main::api::corpus::countEntities();
}

extern "C"
INT get_train_total() {
    return main::api::corpus::countTriples(subset::Type::train);
}

extern "C"
INT get_test_total() {
    return main::api::corpus::countTriples(subset::Type::test);
}

extern "C"
INT get_valid_total() {
    return main::api::corpus::countTriples(subset::Type::valid);
}

