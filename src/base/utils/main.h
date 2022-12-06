#ifndef UTILS_MAIN_H
#define UTILS_MAIN_H

#include "../triple/main.h"

#define invalidArgument invalid_argument

using namespace std;

enum SubsetType { train, test, valid };

SubsetType decodeSubsetType(string name);

struct File {
    FILE* file;
    INT length;

    File(FILE* file, INT length) {
        this->file = file;
        this->length = length;
    }

    void close() {
        fclose(this->file);
    }
};

File* readNumberOfElements(std::string path, bool verbose = false);
File* readNumberOfTriples(std::string path, bool verbose = false);
File* readNumberOfTypeConstrainedRelations(std::string path, bool verbose = false);

#endif
