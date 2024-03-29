#ifndef TRIPLE_READER_H
#define TRIPLE_READER_H
#include <iostream>

#include "main.h"
#include "TripleIndex.h"

struct TripleIds {
    INT last_relation;
    INT last_entity;
    INT last_triple;
};

// extern INT* current_internal_entity_id;
// extern INT* current_internal_relation_id;
// extern INT* current_triple_id;

TripleIds readTriples(FILE* input_file, INT length, bool enable_filters, Triple* tripleList, TripleIndex* tripleIndex, INT start_internal_entity_id = -1, INT start_internal_relation_id = -1);

#endif
