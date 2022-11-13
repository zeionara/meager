#ifndef UTILS_H
#define UTILS_H

#include "Triple.h"

enum InputFileType { idMapping, train, test, valid, typeConstraint };

void readNumberOfElements(TripleComponent component = entity, bool verbose = false);

std::string getInputPath(InputFileType fileType, TripleComponent component = entity);

#endif
