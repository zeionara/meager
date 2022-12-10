#ifndef TRIPLE_LIST_RELATION_SCORE_H
#define TRIPLE_LIST_RELATION_SCORE_H

// #include "../main.h"

#include "TripleList.h"
#include "../Frequencies.h"

using namespace std;

namespace meager::main::triple {

    struct RelationScore {
        REAL* head; 
        REAL* tail;

        INT length;

        RelationScore(List* headList, List* tailList, Frequencies* frequencies) {
            REAL* head = new REAL[frequencies->nRelations];  // (REAL *)calloc(frequencies->nRelations, sizeof(REAL));
            REAL* tail = new REAL[frequencies->nRelations];  // (REAL *)calloc(frequencies->nRelations, sizeof(REAL));

            for (INT i = 0; i < frequencies->nEntities; i++) {
                handleEntity(headList, i, [&](INT relation){head[relation] += 1.0;});
                handleEntity(tailList, i, [&](INT relation){tail[relation] += 1.0;});
            }

            for (INT i = 0; i < frequencies->nRelations; i++) {
                INT relationFrequency = frequencies->relation[i];
                head[i] = double(relationFrequency) / head[i]; // The greater the value the fewer unique heads the relation connects
                tail[i] = double(relationFrequency) / tail[i]; // The greater the value the fewer unique tails the relation connects
            }

            this->head = head;
            this->tail = tail;
            this->length = frequencies->nRelations;
        }

        REAL handleEntity(List* list, INT entity, function<void(INT)> incrementScore) {
            if (list->left[entity] == -1 && list->right[entity] == -1)  {
                return 0;
            }

            REAL value = 0;

            for (INT j = list->left[entity] + 1; j <= list->right[entity]; j++)
                if (list->items[j].r != list->items[j - 1].r) // Count number of entities "emitting" or "receiving" each relation
                    incrementScore(list->items[j].r);

            if (list->left[entity] <= list->right[entity])
                incrementScore(list->items[list->left[entity]].r);

            return value;
        }

        void print() {
            cout << "--- " << " relation scores"  << endl;
            // int i = 0;
            for (INT i = 0; i < this->length; i++) {
                cout << "| relation = " << i << ", head score = " << this->head[i] << ", tail score = " << this->tail[i] << "|" << endl;
            }
            cout << "---" << endl;
        }
    };

}

#endif
