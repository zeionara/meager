#include <algorithm>
#include "main.h"
#include "Index.h"

#include "Triple.h"

#define invalidArgument invalid_argument

using namespace std;

namespace meager::main::triple {

    Component decodeComponent(string name) {

        if (name == "head") {
            return Component::head;
        }

        if (name == "tail") {
            return Component::tail;
        }

        if (name == "relation") {
            return Component::relation;
        }

        throw invalidArgument("Unknown triple element name: " + name);
    }

    void print_triples(string header, Triple* triples, int nTriples) {
        cout << header << "\n";
        for (int i = 0; i < nTriples; i++) {
            triples[i].print();
        }
    }

}
