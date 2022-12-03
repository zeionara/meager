#ifndef CORRUPT_CORRUPTION_STRATEGY_H
#define CORRUPT_CORRUPTION_STRATEGY_H

#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"

#include "../storage/LocalTsvCorpus.h"
// #include "state/RandomizationState.h"
#include "randomizer/Randomizer.h"

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

    virtual Triple corruptHead(Triple triple, Randomizer<INT>* randomizer) = 0;
    virtual Triple corruptTail(Triple triple, Randomizer<INT>* randomizer) = 0;
    virtual Triple corruptRelation(Triple triple, Randomizer<INT>* randomizer) = 0;

    virtual Triple corrupt(Triple triple, Randomizer<INT>* randomizer) = 0;
};

#endif
