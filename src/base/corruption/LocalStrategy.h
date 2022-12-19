#ifndef CORRUPT_DEFAULT_CORRUPTION_STRATEGY_H
#define CORRUPT_DEFAULT_CORRUPTION_STRATEGY_H

// #include "../triple/list/ThickWrapper.h"
// #include "../triple/list/ThinWrapper.h"
#include "../corpus/local/Default.h"
#include "Strategy.h"
#include "randomizer/ConstrainedShiftingRandomizer.h"
// #include "../Random.h"
// #include "state/RandomizationState.h"
// #include "../Reader.h"

namespace meager::main::corruption {

    template <typename T, typename R>
    struct LocalStrategy: Strategy<R> {

        // DefaultCorruptionStrategy(ThickTripleListWrapper* triples, INT threadId, ThinTripleListWrapper* testTriples, ThinTripleListWrapper* validTriples):
        //     CorruptionStrategy(triples, threadId, testTriples, validTriples) {};

        corpus::local::Default<T>* corpus;

        LocalStrategy(corpus::local::Default<T>* corpus, INT threadId, Randomizer<R>* randomizer): Strategy<R>(threadId, randomizer) {
            this->corpus = corpus;
        };

        INT corruptEntity(
            triple::list::List* list, triple::Triple triple, INT maxId,
            INT (*getPrimaryTripleComponent)(triple::Triple triple), INT (*getSecondaryTripleComponent)(triple::Triple triple), INT (*getCorruptableTripleComponent)(triple::Triple triple)
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

            // auto randomizer = new ConstrainedShiftingRandomizer(this->randomizer->state);
            // randomizer->initNextInIntervalExcludingContext(0, maxId, list->items + ll, rr - ll + 1);

            // auto value = randomizer->next();

            // delete randomizer;

            // return value;

            //
            //
            //

            // cout << "Detected bounds " << ll << " " << rr << endl;

            // cout << "Finishing corruption" << endl;

            // INT tmp = rand_max(randomizer, maxId - (rr - ll + 2)); // Generate random entity index in the interval [0; nEntities - (nTailEntitiesForGivenHead + nHeadEntitiesForGivenHead)]
            // INT tmp = randomizer->state->sample(maxId - (rr - ll + 2)); // Generate random entity index in the interval [0; nEntities - (nTailEntitiesForGivenHead + nHeadEntitiesForGivenHead)]
            INT nExcludedItems = rr - ll + 1;

            INT tmp = this->randomizer->state->sample(maxId - nExcludedItems); // Generate random entity index in the interval [0; nEntities - (nTailEntitiesForGivenHead + nHeadEntitiesForGivenHead)]
            if (tmp < getCorruptableTripleComponent(list->items[ll])) return tmp; // If generated entity index is less than any other tail entity index (in other case the generated triple would probably not be unique) then return this
            // if (tmp + rr - ll + 1 > trainHead[rr].t) return tmp + rr - ll + 1;
            if (tmp > getCorruptableTripleComponent(list->items[rr]) - nExcludedItems) return tmp + nExcludedItems; // If generated entity index + max possible offset is larger than any other tail entity index then return this

            lef = ll, rig = rr + 1;
            while (lef + 1 < rig) { // While the left and right boundaries do not overlap (they differ at least by one)
                // cout << "8888" << endl;
                mid = (lef + rig) >> 1;
                // if (trainHead[mid].t + (ll - mid) - 1 < tmp)
                // if (trainHead[mid].t < tmp + (mid - ll) + 1)
                if (getCorruptableTripleComponent(list->items[mid]) - (mid - ll + 1) < tmp)
                    lef = mid;
                else 
                    rig = mid;
            } // Find an optimal value of tmp + offset which does not overlap with present triples
            return tmp + lef - ll + 1;
        }

        triple::Triple corruptHead(triple::Triple triple) {
            INT corruptedHead = corruptEntity(
                corpus->train->tail, triple, corpus->train->frequencies->nEntities,
                [](triple::Triple triple){return triple.t;}, [](triple::Triple triple){return triple.r;}, [](triple::Triple triple){return triple.h;}
            );
            return triple::Triple(corruptedHead, triple.r, triple.t);
        }

        triple::Triple corruptTail(triple::Triple triple) {
            INT corruptedTail = corruptEntity(
                corpus->train->head, triple, corpus->train->frequencies->nEntities,
                [](triple::Triple triple){return triple.h;}, [](triple::Triple triple){return triple.r;}, [](triple::Triple triple){return triple.t;}
            );
            return triple::Triple(triple.h, triple.r, corruptedTail);
        }

        triple::Triple corruptRelation(triple::Triple triple) {
            INT corruptedRelation = corruptEntity(
                corpus->train->relation, triple, corpus->train->frequencies->nRelations,
                [](triple::Triple triple){return triple.h;}, [](triple::Triple triple){return triple.t;}, [](triple::Triple triple){return triple.r;}
            );
            return triple::Triple(triple.h, corruptedRelation, triple.t);
        }

        triple::Triple corrupt(triple::Triple triple){  // Corrupt tail
            INT loop = 0;
            // INT t;
            while(1) {
                // INT corruptedTail = corpus->types->get(triple.r)->tails->items[rand(0, corpus->types->get(triple.r)->tails->length)]; // Select random tail entity id for a given relation according to type mappings
                INT corruptedTail = corpus->types->get(triple.r)->tails->items[this->randomizer->state->sample(corpus->types->get(triple.r)->tails->length)]; // Select random tail entity id for a given relation according to type mappings
                triple::Triple corruptedTriple = triple::Triple(triple.h, triple.r, corruptedTail);
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

        private: bool isCorrectTriple(triple::Triple triple) { // Check whether a triple is presented in the dataset (tripleList variables contains triples from all subsets)
            return corpus->train->index->contains(triple) || corpus->test->index->contains(triple) || corpus->valid->index->contains(triple);
        }
    };

}

#endif
