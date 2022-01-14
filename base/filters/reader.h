#ifndef FILTERS_READER
#define FILTERS_READER

#include <cstdlib>
#include <regex>
#include <vector>
#include <string>

std::vector<std::regex> readFilterPatterns(std::string filename, bool verbose = false, bool drop_duplicates = true);

#endif

