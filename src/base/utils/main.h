#ifndef UTILS_MAIN_H
#define UTILS_MAIN_H

#include <fstream>

#include "../triple/main.h"

#define invalidArgument invalid_argument

using namespace std;

namespace meager::main::utils {

    enum class SubsetType {
        train,
        test,
        valid
    };

    SubsetType decodeSubsetType(string name);

    struct File {
        string path;
        ifstream stream;
        INT length;

        // File(ifstream* stream, INT length) {
        //     this->stream = stream;
        //     this->length = length;
        // }

        File(string path, bool verbose) {
            this->stream.open(path);

            if (!stream.is_open()) {
                cerr << "file " << path << " does not exist" << endl;
                throw invalidArgument("File " + path + " does not exist");
            }

            stream >> length;
            this->path = path;

            // INT foo;

            // stream >> foo;

            // cout << foo << endl;
        }

        void close() {
            stream.close();
        }

        friend File operator>>(File file, long value);
    };

    // File* readNumberOfElements(std::string path, bool verbose = false);
    // File* readNumberOfTriples(std::string path, bool verbose = false);
    // File* readNumberOfTypeConstrainedRelations(std::string path, bool verbose = false);

}


#endif
