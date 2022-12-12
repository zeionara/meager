#include "main.h"

#define invalidArgument

namespace meager::main::triple::pattern {

    namespace inverse {

        const string name = "inverse";
        
    }

    namespace symmetric {

        const string name = "symmetric";
        
    }

    namespace none {

        const string name = "none";
        
    }

    Pattern decodeName(string name) {

        if (name == none::name || name == "nil") {
            return Pattern::None;
        }

        if (name == inverse::name) {
            return Pattern::Inverse;
        }

        if (name == symmetric::name) {
            return Pattern::Symmetric;
        }

        // cerr << "Pattern name " << name << " is unknown" << endl;
        throw invalidArgument("Unknown triple pattern " + name + " provided");
    }

    string getName(Pattern pattern) {
        switch (pattern) {
            case Pattern::None:
                return none::name;
            case Pattern::Symmetric:
                return symmetric::name;
            case Pattern::Inverse:
                return inverse::name;
            default:
                throw invalidArgument("Cannot get name of given pattern");
        }
    }

}

