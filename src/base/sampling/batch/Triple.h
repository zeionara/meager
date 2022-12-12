#ifndef SAMPLING_BATCH_TRIPLE_H
#define SAMPLING_BATCH_TRIPLE_H

// #include "../../triple/main.h"
#include "../../triple/Triple.h"

using namespace std;

namespace meager::main::sampling::batch {

    namespace label {

        const int POSITIVE = 1;

    }

    struct Triple {
        triple::Triple* triples;
        REAL* labels;

        INT length;

        INT* head;
        INT* relation;
        INT* tail;

        Triple(triple::Triple* triples, INT length) {
            REAL* labels = new REAL[length];
            // REAL* labels = (REAL*) malloc(length * sizeof(REAL));

            for (INT i = 0; i < length; i++) {
                labels[i] = label::POSITIVE;
            }

            init(triples, labels, length);
        };

        Triple(triple::Triple* triples, REAL* labels, INT length) {
            init(triples, labels, length);
        }

        ~Triple() {
            // cout << "Releasing memory occupied by triple batch" << endl;
            delete [] triples;
            delete [] labels;

            delete [] head;
            delete [] relation;
            delete [] tail;
            // cout << "Released memory occupied by triple batch" << endl;
        }

        void init(triple::Triple* triples, REAL* labels, INT length) {
            // head = (INT*) malloc(length * sizeof(INT));
            // relation = (INT*) malloc(length * sizeof(INT));
            // tail = (INT*) malloc(length * sizeof(INT));
            head = new INT[length];
            relation = new INT[length];
            tail = new INT[length];

            for (INT i = 0; i < length; i++) {
                triple::Triple triple = triples[i];

                head[i] = triple.h;
                relation[i] = triple.r;
                tail[i] = triple.t;
            }

            this->triples = triples;
            this->labels = labels;

            this->length = length;
        }
    };

}

#endif
