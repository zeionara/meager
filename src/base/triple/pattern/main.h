#ifndef TRIPLE_PATTERN_MAIN_H
#define TRIPLE_PATTERN_MAIN_H

#include <vector>
#include <unordered_set>

// #include "../triple/main.h"
#include "../Triple.h"

using namespace std;
using namespace meager::main;


namespace meager::main::triple::pattern {

    namespace inverse {

        extern
        const string name;
        
    }

    namespace symmetric {

        extern
        const string name;
        
    }

    namespace none {

        extern
        const string name;
        
    }

    enum class Pattern {
        None = 0,
        Symmetric = 1,
        Inverse = 2
    };

    struct Instance {
        vector<triple::Triple> triples;
        unordered_set<int> observedTripleIndices;

        string getObservanceMark(int i) {
            return observedTripleIndices.find(i) == observedTripleIndices.end() ? " - " : " + ";
        }

        string getConciseDescription() {
            string result = "";
            // int i = 0;
            for (triple::Triple triple: triples) {
                result += to_string(triple.h) + "|" + to_string(triple.r) + "|" + to_string(triple.t) + "\n";
            }
            return result;
        }

        void print(string label) {
            cout << "--- " << label << " triples pattern instance"  << endl;
            int i = 0;
            for (triple::Triple triple: triples) {
                cout << "| head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(i) << endl;
            }
            cout << "---" << endl;
        }
    };

    Pattern decodeName(string name);
    string getName(Pattern pattern);

}

#endif
