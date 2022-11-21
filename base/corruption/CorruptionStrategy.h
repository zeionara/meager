#ifndef CORRUPT_CORRUPTION_STRATEGY_H
#define CORRUPT_CORRUPTION_STRATEGY_H

#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"

struct CorruptionStrategy {
    ThickTripleListWrapper* triples;

    ThinTripleListWrapper* testTriples;
    ThinTripleListWrapper* validTriples;

    INT threadId;

    CorruptionStrategy(ThickTripleListWrapper* triples, INT threadId, ThinTripleListWrapper* testTriples, ThinTripleListWrapper* validTriples) {
        this->triples = triples;
        this->threadId = threadId;

        this->testTriples = testTriples;
        this->validTriples = validTriples;
    }

    virtual Triple corruptHead(Triple triple) = 0;
    virtual Triple corruptTail(Triple triple) = 0;
    virtual Triple corruptRelation(Triple triple) = 0;

    virtual Triple corrupt(Triple triple) = 0;
};

#endif
