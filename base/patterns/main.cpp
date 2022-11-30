#include "main.h"

#define invalidArgument

Pattern decodePatternName(string name) {

    if (name == "none" || name == "nil") {
        return none;
    }

    if (name == "inverse") {
        return inverse;
    }

    if (name == "symmetric") {
        return symmetric;
    }

    cerr << "Pattern name " << name << " is unknown" << endl;
    throw invalidArgument("Unknown triple pattern provided");
}
