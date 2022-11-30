#include "../triple/main.h"
#include "main.h"

using namespace std;

#define invalidArgument invalid_argument

File* readHeader(std::string absolutePath, bool verbose, std::function<void(INT)> printMessage) {
	FILE *input_file;
	INT length;

	input_file = fopen(absolutePath.c_str(), "r");

	if (input_file == nullptr) {
	 	cerr << "file " << absolutePath << " does not exist" << endl;
        throw invalidArgument("File does not exist");
	}

	fscanf(input_file, "%ld", &length);

    if (verbose) {
        printMessage(length);
    }

    return new File(input_file, length);
}


File* readNumberOfTriples(std::string path, bool verbose) {
    return readHeader(
        path, verbose,
        [&](INT result){
            printf("The total number of %s triples is %ld.\n", path.c_str(), result);
        }
    );
}

File* readNumberOfTypeConstrainedRelations(std::string path, bool verbose) {
    return readHeader(
        path, verbose,
        [&](INT result){
            printf("The total number of type constrained relations is %ld.\n", result);
        }
    );
}

File* readNumberOfElements(std::string path, bool verbose) {
    return readHeader(
        path, verbose,
        [&](INT result){
            printf("The total number of %s is %ld.\n", path.c_str(), result);
        }
    );
}
