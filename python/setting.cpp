#include "../base/Setting.h"

extern "C"
void set_in_path(char *path, bool as_tsv = false) {
    setInPath(path, as_tsv);
}

extern "C"
void set_bern(INT con) {
    setBern(con);
}

extern "C"
void set_work_threads(INT threads) {
    setWorkThreads(threads);
}

extern "C"
INT get_relation_total() {
    return getRelationTotal();
}

extern "C"
INT get_entity_total() {
    return getEntityTotal();
}

extern "C"
INT get_train_total() {
    return getTrainTotal();
}

extern "C"
INT get_test_total() {
    return getTestTotal();
}

extern "C"
INT get_valid_total() {
    return getValidTotal();
}

