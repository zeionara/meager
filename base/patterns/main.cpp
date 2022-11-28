#include "main.h"

unordered_map<Pattern, PatternDescription> patternDescriptions;

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

    cout << "Pattern name " << name << " is unknown" << endl;
    throw "Unknown triple pattern provided";
}
