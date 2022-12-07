#include "main.h"

namespace meager::main::utils {

    SubsetType decodeSubsetType(string name) {

        if (name == "train") {
            return SubsetType::train;
        }

        if (name == "test") {
            return SubsetType::test;
        }

        if (name == "valid") {
            return SubsetType::valid;
        }

        throw invalidArgument("Unknown corpus subset provided: " + name);
    }

}
