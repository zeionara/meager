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

    // class File {

    // protected:
    //         ifstream stream;

    // public:
    //     string path;

    //     File(string path, bool verbose) {
    //         this->stream.open(path);

    //         if (!stream.is_open()) {
    //             cerr << "file " << path << " does not exist" << endl;
    //             throw invalidArgument("File " + path + " does not exist");
    //         }

    //         this->path = path;
    //     }

    //     void close() {
    //         stream.close();
    //     }

    //     bool good() {
    //         return stream.good();
    //     }

    //     friend File& operator>>(File& file, long value);
    // };

    // class FileWithHeader: public File {

    // public:
    //     INT length;

    //     FileWithHeader(string path, bool verbose): File(path, verbose) {
    //         *this >> length;
    //     }

    // };

    struct File {

        ifstream stream;

        string path;

        File(string path, bool verbose) {
            this->stream.open(path);

            if (!stream.is_open()) {
                cerr << "file " << path << " does not exist" << endl;
                throw invalidArgument("File " + path + " does not exist");
            }

            this->path = path;
        }

        void close() {
            stream.close();
        }

        bool good() {
            return stream.good();
        }

        friend File& operator>>(File& file, long value);
    };

    struct FileWithHeader: File {

        INT length;

        FileWithHeader(string path, bool verbose): File(path, verbose) {
            *this >> length;
        }

    };
}

#endif
