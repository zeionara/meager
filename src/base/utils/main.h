#ifndef UTILS_MAIN_H
#define UTILS_MAIN_H

#include <fstream>
#include <functional>

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

    class File {

    protected:
        ifstream stream;

    public:
        string path;

        File(string path, bool verbose) {
            this->stream.open(path);

            if (!stream.is_open()) {
                throw invalidArgument("Cannot open file" + path);
            }

            this->path = path;
        }

        bool getLine(string line) {
            return (bool)getline(this->stream, line);
        }

        ~File() {
            stream.close();
        }

        bool good() {
            return stream.good();
        }

        friend File& operator>>(File& file, long& value);
    };

    class FileWithHeader: public File {

    public:
        INT length;

        FileWithHeader(string path, bool verbose): File(path, verbose) {
            *this >> length;
        }

    };

}

#endif
