#ifndef UTILS_H
#define UTILS_H

#include "../triple/main.h"

enum InputFileType { idMapping, triples, typeConstraint };
enum SubsetType { train, test, valid };

void readNumberOfElements(TripleComponent component = entity, bool verbose = false);
FILE* readNumberOfTriples(SubsetType subsetType = train, bool verbose = false);

std::string getInputPath(InputFileType fileType, TripleComponent component);
std::string getInputPath(InputFileType fileType, SubsetType subsetType);
std::string getInputPath(InputFileType fileType);

#endif
