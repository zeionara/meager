#ifndef CORRUPTION_STRATEGY_H
#define CORRUPTION_STRATEGY_H

// #include "../triple/list/ThickWrapper.h"
// #include "../triple/list/ThinWrapper.h"

// #include "../storage/LocalTsvCorpus.h"
// #include "state/RandomizationState.h"
#include "../triple/Triple.h"
#include "randomizer/Randomizer.h"

namespace meager::main::corruption {

    struct Strategy {
        // ThickTripleListWrapper* triples;

        // ThinTripleListWrapper* testTriples;
        // ThinTripleListWrapper* validTriples;

        // LocalTsvCorpus* corpus;

        INT threadId;
        Randomizer<INT>* randomizer;

        Strategy(INT threadId, Randomizer<INT>* randomizer) {
            // this->triples = triples;
            // this->corpus = corpus;
            this->threadId = threadId;
            this->randomizer = randomizer;

            // this->testTriples = testTriples;
            // this->validTriples = validTriples;
        }

        virtual triple::Triple corruptHead(triple::Triple triple) = 0;
        virtual triple::Triple corruptTail(triple::Triple triple) = 0;
        virtual triple::Triple corruptRelation(triple::Triple triple) = 0;

        virtual triple::Triple corrupt(triple::Triple triple) = 0;
    };

}

#endif
