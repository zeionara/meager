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

    virtual triple::Triple corruptHead(triple::Triple triple, Randomizer<INT>* randomizer) = 0;
    virtual triple::Triple corruptTail(triple::Triple triple, Randomizer<INT>* randomizer) = 0;
    virtual triple::Triple corruptRelation(triple::Triple triple, Randomizer<INT>* randomizer) = 0;

    virtual triple::Triple corrupt(triple::Triple triple, Randomizer<INT>* randomizer) = 0;
};

#endif
