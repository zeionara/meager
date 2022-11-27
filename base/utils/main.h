#ifndef UTILS_H
#define UTILS_H

#include "../triple/main.h"

enum InputFileType { idMapping, triples, typeConstraint };
enum SubsetType { train, test, valid };

std::string getInputPath(InputFileType fileType, TripleComponent component);
std::string getInputPath(InputFileType fileType, SubsetType subsetType);
std::string getInputPath(InputFileType fileType);

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

File* readNumberOfElements(TripleComponent component = entity, std::string path = "", bool verbose = false);
File* readNumberOfElements(std::string path, bool verbose);

File* readNumberOfTriples(SubsetType subsetType = train, std::string path = "", bool verbose = false);
File* readNumberOfTriples(std::string path, bool verbose = false);

// File* readNumberOfTypeConstrainedRelations(bool verbose);
File* readNumberOfTypeConstrainedRelations(std::string path, bool verbose);

#endif
