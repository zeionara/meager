#include "../triple/main.h"

#include "main.h"

#define invalidArgument invalid_argument

using namespace std;

namespace meager::main::utils {

    File& operator>>(File& file, long value) {
        file.stream >> value;
        return file;
    }

        // File* printFileLength(File* file) {
        //     printf("The number of elements in file '%s' is %ld.\n", file->path.c_str(), file->length);
        //     return file;
        // }

        // File* readHeader(std::string absolutePath, bool verbose, std::function<void(INT)> printMessage) {
        //     // FILE *inputFile;
        //     // INT length;

        //     // ifstream stream(absolutePath.c_str(), ifstream::in);

        //     // inputFile = fopen(absolutePath.c_str(), "r");

        //     if (!stream.is_open()) {
        //         cerr << "file " << absolutePath << " does not exist" << endl;
        //         throw invalidArgument("File " + absolutePath + " does not exist");
        //     }

        //     stream >> length;

        //     if (verbose) {
        //         printMessage(length);
        //     }

        //     return new File(inputFile, length);
        // }


        // File* readNumberOfTriples(string path, bool verbose) {
        //     File* file = new File(path);

        //     if (verbose)
        //         printFileLength(file);

        //     return file;
        // }

        // File* readNumberOfTypeConstrainedRelations(std::string path, bool verbose) {
        //     File* file = new File(path);

        //     if (verbose)
        //         printf("The total number of type constrained relations is %ld.\n", file->length);

        //     return file;
        // }

        // File* readNumberOfElements(std::string path, bool verbose) {
        //     File* file = new File(path);

        //     if (verbose)
        //         printf("The total number of %s is %ld.\n", path.c_str(), file->length);

        //     return file;
        // }

}
