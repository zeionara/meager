// #include "../base/Setting.h"
#include "../base/api/corpus.h"

using namespace meager::main::utils;

extern "C"
void set_in_path(char *path, bool as_tsv = false) {
    initCorpus(path);
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
    return countRelations();
}

extern "C"
INT get_entity_total() {
    return countEntities();
}

extern "C"
INT get_train_total() {
    return countTriples(subset::Type::train);
}

extern "C"
INT get_test_total() {
    return countTriples(subset::Type::test);
}

extern "C"
INT get_valid_total() {
    return countTriples(subset::Type::valid);
}

