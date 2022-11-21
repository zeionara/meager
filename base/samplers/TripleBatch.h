#ifndef TRIPLE_BATCH_H
#define TRIPLE_BATCH_H

#include "../triple/main.h"

struct TripleBatch {
    Triple* triples;
    REAL* labels;

    INT length;

    INT* head;
    INT* relation;
    INT* tail;

    TripleBatch(Triple* triples, INT length) {
        REAL* labels = (REAL*) malloc(length * sizeof(REAL));

        for (INT i = 0; i < length; i++)
            labels[i] = 1;

        TripleBatch(triples, labels, length);
    };

    TripleBatch(Triple* triples, REAL* labels, INT length) {
        head = (INT*) malloc(length * sizeof(INT));
        relation = (INT*) malloc(length * sizeof(INT));
        tail = (INT*) malloc(length * sizeof(INT));

        for (INT i = 0; i < length; i++) {
            Triple triple = triples[i];

            head[i] = triple.h;
            relation[i] = triple.r;
            tail[i] = triple.t;
        }

        this->triples = triples;
        this->labels = labels;

        this->length = length;
    }
};

#endif
