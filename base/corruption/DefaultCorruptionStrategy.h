#ifndef CORRUPT_DEFAULT_CORRUPTION_STRATEGY_H
#define CORRUPT_DEFAULT_CORRUPTION_STRATEGY_H

#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
#include "CorruptionStrategy.h"
#include "../Random.h"
// #include "../Reader.h"

template <typename T>
struct DefaultCorruptionStrategy: CorruptionStrategy {

    // DefaultCorruptionStrategy(ThickTripleListWrapper* triples, INT threadId, ThinTripleListWrapper* testTriples, ThinTripleListWrapper* validTriples):
    //     CorruptionStrategy(triples, threadId, testTriples, validTriples) {};

    LocalTsvCorpus<T>* corpus;

    DefaultCorruptionStrategy(LocalTsvCorpus<T>* corpus, INT threadId): CorruptionStrategy(threadId) {
        this->corpus = corpus;
    };

    INT corruptEntity(
        TripleList* list, Triple triple, INT maxId,
        INT (*getPrimaryTripleComponent)(Triple triple), INT (*getSecondaryTripleComponent)(Triple triple), INT (*getCorruptableTripleComponent)(Triple triple)
    ) {

        // cout << "Corrupting..." << endl;

        INT lef, rig, mid, ll, rr;

        INT primaryTripleComponent = getPrimaryTripleComponent(triple);
        INT secondaryTripleComponent = getSecondaryTripleComponent(triple);

        lef = list->left[primaryTripleComponent] - 1;
        rig = list->right[primaryTripleComponent];

        // cout << lef << "**" << rig << endl;

        // cout << "Some elements from the triples list: " << getPrimaryTripleComponent(list->items[1000]) << " " << getPrimaryTripleComponent(list->items[2000]) << " " << getPrimaryTripleComponent(list->items[3000]) << endl;

        while (lef + 1 < rig) { // Find location of the first triple having required relationship and save it to the rig (and subsequently - to the ll) variable
            mid = (lef + rig) >> 1;
            if (getSecondaryTripleComponent(list->items[mid]) >= secondaryTripleComponent) rig = mid; else // If same relationship then move right boundary closer to the left (i.e. closer to the beginning of the list)
            lef = mid;
        }
        ll = rig;

        lef = list->left[primaryTripleComponent];
        rig = list->right[primaryTripleComponent] + 1;

        while (lef + 1 < rig) { // Find location of the last triple having required relationship and save it to the lef (and subsequently - to the rr) variable
            mid = (lef + rig) >> 1;
            if (getSecondaryTripleComponent(list->items[mid].r) <= secondaryTripleComponent) lef = mid; else // If same relationship then move left boundary closer to the right (i.e. closer to the ending of the list)
            rig = mid;
        }
        rr = lef;

        // cout << "Detected bounds " << ll << " " << rr << endl;

        // cout << "Finishing corruption" << endl;

        INT tmp = rand_max(threadId, maxId - (rr - ll + 2)); // Generate random entity index in the interval [0; nEntities - (nTailEntitiesForGivenHead + nHeadEntitiesForGivenHead)]
        if (tmp < getCorruptableTripleComponent(list->items[ll])) return tmp; // If generated entity index is less than any other tail entity index (in other case the generated triple would probably not be unique) then return this
        // if (tmp + rr - ll + 1 > trainHead[rr].t) return tmp + rr - ll + 1;
        if (tmp > getCorruptableTripleComponent(list->items[rr]) - rr + ll - 1) return tmp + rr - ll + 1; // If generated entity index + max possible offset is larger than any other tail entity index then return this

        lef = ll, rig = rr + 1;
        while (lef + 1 < rig) { // While the left and right boundaries do not overlap (they differ at least by one)
            // cout << "8888" << endl;
            mid = (lef + rig) >> 1;
            // if (trainHead[mid].t + (ll - mid) - 1 < tmp)
            // if (trainHead[mid].t < tmp + (mid - ll) + 1)
            if (getCorruptableTripleComponent(list->items[mid]) - mid + ll - 1 < tmp)
                lef = mid;
            else 
                rig = mid;
        } // Find an optimal value of tmp + offset which does not overlap with present triples
        return tmp + lef - ll + 1;
    }

    Triple corruptHead(Triple triple) {
        INT corruptedHead = corruptEntity(
            corpus->train->tail, triple, corpus->train->frequencies->nEntities,
            [](Triple triple){return triple.t;}, [](Triple triple){return triple.r;}, [](Triple triple){return triple.h;}
        );
        return Triple(corruptedHead, triple.r, triple.t);
    }

    Triple corruptTail(Triple triple) {
        INT corruptedTail = corruptEntity(
            corpus->train->head, triple, corpus->train->frequencies->nEntities,
            [](Triple triple){return triple.h;}, [](Triple triple){return triple.r;}, [](Triple triple){return triple.t;}
        );
        return Triple(triple.h, triple.r, corruptedTail);
    }

    Triple corruptRelation(Triple triple) {
        INT corruptedRelation = corruptEntity(
            corpus->train->relation, triple, corpus->train->frequencies->nRelations,
            [](Triple triple){return triple.h;}, [](Triple triple){return triple.t;}, [](Triple triple){return triple.r;}
        );
        return Triple(triple.h, corruptedRelation, triple.t);
    }

    Triple corrupt(Triple triple){  // Corrupt tail
        INT loop = 0;
        INT t;
        while(1) {
            INT corruptedTail = corpus->types->get(triple.r)->tails->items[rand(0, corpus->types->get(triple.r)->tails->length)]; // Select random tail entity id for a given relation according to type mappings
            Triple corruptedTriple = Triple(triple.h, triple.r, corruptedTail);
            if (not isCorrectTriple(corruptedTriple)) { // If obtained triple does not exist, then it is a suitable negative sample which may be immediately returned
            //	printf("r:%ld\tt:%ld\n", r, t);
                return corruptedTriple;
            } else { // If not, then repeat the stochastic approach 1000 times and if it still doesn't work, then try to apply a more precise algorithm
                loop ++;
                if (loop >= 1000){
                //	printf("drop\n");
                    return corruptTail(triple);
                }
            } 
        }
    }

    private: bool isCorrectTriple(Triple triple) { // Check whether a triple is presented in the dataset (tripleList variables contains triples from all subsets)
        return corpus->train->index->contains(triple) || corpus->test->index->contains(triple) || corpus->valid->index->contains(triple);
    }
};

#endif
