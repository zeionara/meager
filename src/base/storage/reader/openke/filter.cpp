#include "OpenKECorpusReader.h"

namespace meager::main::storage::reader::openke {

    vector<regex> Corpus::readFilterPatterns(bool excluding, bool verbose, bool dropDuplicates) {
        File file = File(path + (excluding ? EXCLUDING_FILTERS_FILENAME : INCLUDING_FILTERS_FILENAME), verbose);

        vector<regex> patterns;

        for (string line; file.getLine(line);) {
            if (dropDuplicates) {
                unorderedSet<string> seenPatterns;

                if (seenPatterns.find(line) == seenPatterns.end()) {
                    seenPatterns.insert(line);
                    regex lineRegex(line);
                    patterns.pushBack(lineRegex);

                    if (verbose) {
                        cout << "read filter pattern " << line << endl;
                    }
                }
            } else {
                regex lineRegex(line);
                patterns.pushBack(lineRegex);
            }
        }

        if (verbose) {
            cout << "finished reading filter patterns" << endl;
        }

        return patterns;
    }

}
