#include "main.h"

namespace meager::main::utils {

    File& operator>>(File& file, long& value) {
        file.stream >> value;
        return file;
    }

    namespace subset {

        Type decodeName(string name) {

            if (name == "train") {
                return Type::train;
            }

            if (name == "test") {
                return Type::test;
            }

            if (name == "valid") {
                return Type::valid;
            }

            throw invalidArgument("Unknown corpus subset provided: " + name);
        }

    }

}
