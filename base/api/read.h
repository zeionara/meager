#ifndef API_READ_H
#define API_READ_H

extern
void importTrain(bool verbose = false);

extern
void importTest(bool verbose = false);

extern
void importValid(bool verbose = false);

extern
void importTypes(bool verbose = false);

extern
void importFilter(bool verbose = false, bool dropDuplicates = true);

#endif
