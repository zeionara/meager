#ifndef TRIPLE_FREQUENCIES_H
#define TRIPLE_FREQUENCIES_H

#include "../types.h"

namespace meager::main::triple {

    struct ComponentFrequencies {
        INT* entity;
        INT* relation;

        INT nEntities;
        INT nRelations;

        ComponentFrequencies(INT nEntities, INT nRelations) {
            this->entity = new INT[nEntities];
            this->relation = new INT[nRelations];

            this->nEntities = nEntities;
            this->nRelations = nRelations;
        }

        ~ComponentFrequencies() {
            delete [] this->entity;
            delete [] this->relation;
        }
    };

}

#endif
