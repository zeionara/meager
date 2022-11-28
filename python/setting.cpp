// #include "../base/Setting.h"
#include "../base/api/corpus.h"

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
    return countTrainTriples();
}

extern "C"
INT get_test_total() {
    return countTestTriples();
}

extern "C"
INT get_valid_total() {
    return countValidTriples();
}

