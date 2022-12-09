#ifndef TRIPLE_INDEX_H
#define TRIPLE_INDEX_H

#include <unordered_map>
#include <unordered_set>

#include "Triple.h"

using namespace std;

namespace meager::main::triple {

    using map = unordered_map<
        INT,
        unordered_map<
            INT,
            unordered_set<INT>
        >
    >;

    struct TripleIndex {
        map triples;

        void push(Triple triple) {
            auto tailsAndRelationsIterator = triples.find(triple.h);

            if (tailsAndRelationsIterator == triples.end()) {  // If haven't seen any triple with such head previously
                triples[triple.h] = {
                    {
                        triple.t, {
                            triple.r
                        }
                    }
                };
            } else { // If have seen a triple with such head previously
                auto tailsAndRelations = tailsAndRelationsIterator->second;
                auto tailsIterator = tailsAndRelations.find(triple.t); 
                if (tailsIterator == tailsAndRelations.end()) {  // If haven't seen any triple with such head and tail previously
                    tailsAndRelations[triple.t] = {triple.r};

                    triples[triple.h] = tailsAndRelations;
                } else { // If have seen a triple with such head and tail previously
                    auto relations = tailsIterator->second;
                    auto iterator = relations.find(triple.r);
                    if (iterator == relations.end()) { // If haven't seen a triple with such head, tail and relation previously
                        relations.insert(triple.r);

                        tailsAndRelations[triple.t] = relations;
                        triples[triple.h] = tailsAndRelations;
                    } // If have seen a triple with such head, tail and relation when don't need to change anything
                }
            }
        }

        bool contains(Triple triple) {
            auto tailsAndRelationsIterator = triples.find(triple.h);

            if (tailsAndRelationsIterator == triples.end()) {
                return false;
            } else {
                auto tailsAndRelations = tailsAndRelationsIterator->second;
                auto tailsIterator = tailsAndRelations.find(triple.t); 
                if (tailsIterator == tailsAndRelations.end()) {
                    return false;
                } else {
                    auto relations = tailsIterator->second;
                    auto iterator = relations.find(triple.r);
                    if (iterator == relations.end()) {
                        return false;
                    }
                    return true;
                }
            }
        }
    };

}

#endif
