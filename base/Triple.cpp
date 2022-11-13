#include <string>
#include "Triple.h"


std::string getPluralTripleComponentName(TripleComponent component) {
    switch (component) {
        case entity:
            return "entities";
        case relation:
            return "relations";
        default:
            return "unknown";
    }
}

