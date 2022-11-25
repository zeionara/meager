#ifndef TRIPLE_LIST_RELATION_SCORE_H
#define TRIPLE_LIST_RELATION_SCORE_H

#include "../main.h"

#include "TripleList.h"
#include "../type.h"

struct RelationScore {
    REAL* head; 
    REAL* tail;

    INT length;

    RelationScore(TripleList* headList, TripleList* tailList, Frequencies* frequencies) {
        // cout << "First" << endl;
        REAL* head = (REAL *)calloc(frequencies->nRelations, sizeof(REAL));
        REAL* tail = (REAL *)calloc(frequencies->nRelations, sizeof(REAL));
        // cout << "Second" << endl;

        // for (INT k = 0; k < headList->length; k++) {
        //     cout << headList->items[k].h << " " << headList->items[k].t << endl;
        // }

        // for (INT l = 0; l < frequencies->nEntities; l++) {
        //     cout << l << " " << headList->left[l] << " " << headList->right[l] << endl;
        // }

        for (INT i = 0; i < frequencies->nEntities; i++) {
            handleEntity(headList, i, [&](INT relation){head[relation] += 1.0;});
            handleEntity(tailList, i, [&](INT relation){tail[relation] += 1.0;});
        }
        // cout << "Third" << endl;

        for (INT i = 0; i < frequencies->nRelations; i++) {
            INT relationFrequency = frequencies->relation[i];
            // INT relationFrequency = 1;
            // if (i < frequencies->nRelations) {
            //     relationFrequency = frequencies->relation[i];
            // }
            head[i] = double(relationFrequency) / head[i]; // The greater the value the fewer unique heads the relation connects
            tail[i] = double(relationFrequency) / tail[i]; // The greater the value the fewer unique tails the relation connects
        }
        // cout << "Fourth" << endl;

        this->head = head;
        this->tail = tail;
        this->length = frequencies->nRelations;
    }

    REAL handleEntity(TripleList* list, INT entity, std::function<void(INT)> incrementScore) {
        if (list->left[entity] == -1 && list->right[entity] == -1)  {
            return 0;
        }

        REAL value = 0;

        // cout << "foo" << endl;

        // cout << list->left[entity] << endl;
        // cout << list->right[entity] << endl;

        for (INT j = list->left[entity] + 1; j <= list->right[entity]; j++)
            if (list->items[j].r != list->items[j - 1].r) // Count number of entities "emitting" or "receiving" each relation
                incrementScore(list->items[j].r);

        // cout << "bar" << endl;

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
