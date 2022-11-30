#ifndef CORRUPT_CORRUPTION_STRATEGY_H
#define CORRUPT_CORRUPTION_STRATEGY_H

#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"

#include "../storage/LocalTsvCorpus.h"
#include "state/RandomizationState.h"

struct CorruptionStrategy {
    // ThickTripleListWrapper* triples;

    // ThinTripleListWrapper* testTriples;
    // ThinTripleListWrapper* validTriples;

    // LocalTsvCorpus* corpus;

    INT threadId;

    CorruptionStrategy(INT threadId) {
        // this->triples = triples;
        // this->corpus = corpus;
        this->threadId = threadId;

        // this->testTriples = testTriples;
        // this->validTriples = validTriples;
    }

    virtual Triple corruptHead(Triple triple, RandomizationState* randomizer) = 0;
    virtual Triple corruptTail(Triple triple, RandomizationState* randomizer) = 0;
    virtual Triple corruptRelation(Triple triple, RandomizationState* randomizer) = 0;

    virtual Triple corrupt(Triple triple, RandomizationState* randomizer) = 0;
};

#endif
