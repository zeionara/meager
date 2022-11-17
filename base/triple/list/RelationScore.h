#ifndef TRIPLE_LIST_RELATION_SCORE_H
#define TRIPLE_LIST_RELATION_SCORE_H

#include "../main.h"

#include "TripleList.h"

struct RelationScore {
    REAL* head; 
    REAL* tail;

    INT length;

    RelationScore(TripleList* headList, TripleList* tailList, Frequencies* frequencies) {
        REAL* head = (REAL *)calloc(frequencies->nRelations, sizeof(REAL));
        REAL* tail = (REAL *)calloc(frequencies->nRelations, sizeof(REAL));

        for (INT i = 0; i < frequencies->nEntities; i++) {
            handleEntity(headList, i, [&](INT relation){head[relation] += 1.0;});
            handleEntity(tailList, i, [&](INT relation){tail[relation] += 1.0;});
        }
        for (INT i = 0; i < frequencies->nRelations; i++) {
            head[i] = frequencies->relation[i] / head[i]; // The greater the value the fewer unique heads the relation connects
            tail[i] = frequencies->relation[i] / tail[i]; // The greater the value the fewer unique tails the relation connects
        }

        this->head = head;
        this->tail = tail;
        this->length = frequencies->nRelations;
    }

    REAL handleEntity(TripleList* list, INT entity, std::function<void(INT)> incrementScore) {
        REAL value = 0;

        for (INT j = list->left[entity] + 1; j <= list->right[entity]; j++)
            if (list->items[j].r != list->items[j - 1].r) // Count number of entities "emitting" or "receiving" each relation
                incrementScore(list->items[j].r);
        if (list->left[entity] <= list->right[entity])
            incrementScore(list->items[list->left[entity]].r);

        return value;
    }

    void print() {
        std::cout << "--- " << " relation scores"  << std::endl;
        int i = 0;
        for (INT i = 0; i < this->length; i++) {
            std::cout << "| relation = " << i << ", head score = " << this->head[i] << ", tail score = " << this->tail[i] << "|" << std::endl;
        }
        std::cout << "---" << std::endl;
    }
};

#endif
