#ifndef CORPUS_CORPUS_H
#define CORPUS_CORPUS_H

#include "../triple/Triple.h"

namespace meager::main::corpus {

    struct Corpus {
        virtual bool contains(triple::Triple triple) = 0;
        virtual bool allows(triple::Triple triple) = 0;
        virtual long countEntities() = 0;
        virtual long countRelations() = 0;
    };

}

#endif
