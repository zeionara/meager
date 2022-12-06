#include "main.h"

SubsetType decodeSubsetType(string name) {

    if (name == "train") {
        return train;
    }

    if (name == "test") {
        return test;
    }

    if (name == "valid") {
        return valid;
    }

    throw invalidArgument("Unknown corpus subset provided: " + name);
}
