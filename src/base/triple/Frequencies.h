#ifndef TRIPLE_FREQUENCIES_H
#define TRIPLE_FREQUENCIES_H

#include "../types.h"

namespace meager::main::triple {

    struct Frequencies {
        INT* entity;
        INT* relation;

        INT nEntities;
        INT nRelations;

        Frequencies(INT nEntities, INT nRelations) {
            this->entity = new INT[nEntities];
            this->relation = new INT[nRelations];

            this->nEntities = nEntities;
            this->nRelations = nRelations;
        }

        ~Frequencies() {
            delete [] this->entity;
            delete [] this->relation;
        }
    };

}

#endif
