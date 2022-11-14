#ifndef TRIPLE_INDEX_H
#define TRIPLE_INDEX_H

#include "../Setting.h"
#include "main.h"

#include <unordered_set>

using namespace std;
using triples_map = unordered_map<
    INT,
    unordered_map<
        INT,
        unordered_set<INT>
    >
>;

struct TripleIndex {
    triples_map triples;

    void add(Triple triple) {
        triples_map::const_iterator relations_and_tails_iterator = triples.find(triple.h); // unordered_map<INT, unordered_set<INT>>

        if (relations_and_tails_iterator == triples.end()) {
            triples[triple.h] = {
                {
                    triple.t, {
                        triple.r
                    }
                }
            }; 
        } else {
            unordered_map<INT, unordered_set<INT>>::const_iterator tails_iterator = relations_and_tails_iterator->second.find(triple.t); 
            if (tails_iterator == relations_and_tails_iterator->second.end()) {
                unordered_map<INT, unordered_set<INT>> relations_and_tails_map = relations_and_tails_iterator->second;
                relations_and_tails_map[triple.t] = {triple.r};

                triples[triple.h] = relations_and_tails_map;
            } else {
                unordered_set<INT>::const_iterator last_iterator = tails_iterator->second.find(triple.r);
                if (last_iterator == tails_iterator->second.end()) {
                    unordered_map<INT, unordered_set<INT>> relations_and_tails_map = relations_and_tails_iterator->second;
                    unordered_set<INT> tails = tails_iterator->second;
                    tails.insert(triple.r);

                    relations_and_tails_map[triple.t] = tails;
                    triples[triple.h] = relations_and_tails_map;
                }
            }
        }
    }

    bool contains(Triple triple) {
        triples_map::const_iterator relations_and_tails_iterator = triples.find(triple.h);

        if (relations_and_tails_iterator == triples.end()) {
            // cout << "No triples with head " << triple.h << endl;
            return false;
        } else {
            unordered_map<INT, unordered_set<INT>>::const_iterator tails_iterator = relations_and_tails_iterator->second.find(triple.t); 
            if (tails_iterator == relations_and_tails_iterator->second.end()) {
                // cout << "No triples with tail " << triple.t << endl;
                return false;
            } else {
                unordered_set<INT>::const_iterator last_iterator = tails_iterator->second.find(triple.r);
                if (last_iterator == tails_iterator->second.end()) {
                    // cout << "No triples with relation " << triple.r << endl;

                    // for (INT tail: tails_iterator->second) {
                    //     cout << tail << endl;
                    // }

                    return false;
                }
                return true;
            }
        }
    }
};

#endif

