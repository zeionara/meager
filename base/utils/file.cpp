#include "../triple/main.h"
#include "main.h"

// enum InputFileType { idMapping, train, test, valid, typeConstraint };

std::string getInputPath(InputFileType fileType, TripleComponent component) {
    switch (fileType) {
        case idMapping:
            switch (component) {
                case entity:
                    return "entity2id.txt";
                case ::TripleComponent::relation:
                    return "relation2id.txt";
            }
        default:
            std::cout << "Given input file type does not support triple component specification" << std::endl;
            return "error";
    }
}

std::string getInputPath(InputFileType fileType, SubsetType subsetType) {
    switch (fileType) {
        case triples:
            switch (subsetType) {
                case train:
                    return "train2id.txt";
                case test:
                    return "test2id.txt";
                case valid:
                    return "valid2id.txt";
            }
        default:
            std::cout << "Given input file type does not support subset type specification" << std::endl;
            return "error";
    }
}

std::string getInputPath(InputFileType fileType) {
    switch (fileType) {
        case typeConstraint:
            return "type_constrain.txt";
        default:
            std::cout << "Given input file type requires additional data" << std::endl;
            return "error";
    }
}

std::string getSubsetTypeName(SubsetType subsetType) {
    switch (subsetType) {
        case train:
            return "train";
        case test:
            return "test";
        case valid:
            return "valid";
        default:
            return "unknown";
    }
}

File* readHeader(std::string relativePath, bool verbose, std::function<void(INT)> printMessage) {
	FILE *input_file;
	INT length;

    // std::string relativePath = getInputPath(idMapping, component);
    // std::string componentName = component == entity ? "entities" : "relations";

	input_file = fopen((inPath + relativePath).c_str(), "r");

	if (input_file == nullptr) {
	 	std::cerr << '`' << inPath << relativePath << '`' << " does not exist" << std::endl;
        throw "File does not exist";
	 	// return new File(input_file, ;
	}

	fscanf(input_file, "%ld", &length);

    if (verbose) {
        printMessage(length);
        // printf("The total number of %s is %ld.\n", getPluralTripleComponentName(component).c_str(), *tmp);
    }

	// fclose(input_file);

    return new File(input_file, length);
}

File* readHeaderUsingAbsolutePath(std::string absolutePath, bool verbose, std::function<void(INT)> printMessage) {
	FILE *input_file;
	INT length;

	input_file = fopen(absolutePath.c_str(), "r");

    // std::cout << "oooooooooooooooooooooooooooo" << std::endl;

	if (input_file == nullptr) {
	 	std::cerr << '`' << absolutePath << '`' << " does not exist" << std::endl;
        throw "File does not exist";
	}

	fscanf(input_file, "%ld", &length);

    // std::cout << "oooooooooooooooooooooooooooo" << std::endl;

    if (verbose) {
        printMessage(length);
    }

    return new File(input_file, length);
}


File* readNumberOfElements(TripleComponent component, bool verbose) {
	// FILE *input_file;
	// INT* tmp;
    // long* total = component == entity ? &entityTotal : &relationTotal;

    std::string relativePath = getInputPath(idMapping, component);
    std::string componentName = component == entity ? "entities" : "relations";

    File* file = readHeader(
        relativePath, verbose,
        [&](INT result){
            printf("The total number of %s is %ld.\n", getPluralTripleComponentName(component).c_str(), result);
        }
    );

    file->close();

    return file;

	// input_file = fopen((inPath + relativePath).c_str(), "r");

	// if (input_file == nullptr) {
	//  	std::cout << '`' << inPath << relativePath << '`' << " does not exist" << std::endl;
	//  	return;
	// }

	// fscanf(input_file, "%ld", tmp);

    // if (verbose) {
    //     printf("The total number of %s is %ld.\n", getPluralTripleComponentName(component).c_str(), *tmp);
    // }

	// fclose(input_file);

    // return *tmp;
}

File* readNumberOfTriples(SubsetType subsetType, bool verbose) {
    // FILE *input_file;
	// int tmp;
    // long* total = subsetType == train ? &trainTotal : subsetType == test ? &testTotal : &validTotal;

    std::string relativePath = getInputPath(triples, subsetType);

    return readHeader(
        relativePath, verbose,
        [&](INT result){
            printf("The total number of %s triples is %ld.\n", getSubsetTypeName(subsetType).c_str(), result);
        }
    );

	// input_file = fopen((inPath + relativePath).c_str(), "r");

	// if (input_file == nullptr) {
	// 	std::cout << '`' << inPath << relativePath << '`' << " does not exist" << std::endl;
	// 	throw std::invalid_argument("File does not exist");
	// }

	// fscanf(input_file, "%ld", total); // Reading number of train samples

    // if (verbose) {
    //     printf("The total number of %s triples is %ld.\n", getSubsetTypeName(subsetType).c_str(), *total);
    // }

    // return input_file;
}

File* readNumberOfTriples(std::string path, bool verbose) {
    return readHeaderUsingAbsolutePath(
        path, verbose,
        [&](INT result){
            printf("The total number of %s triples is %ld.\n", path.c_str(), result);
        }
    );
}

File* readNumberOfTypeConstrainedRelations(bool verbose) {
    // FILE *input_file;
	// int tmp;
    // long* total = subsetType == train ? &trainTotal : subsetType == test ? &testTotal : &validTotal;

    std::string relativePath = getInputPath(typeConstraint);

    return readHeader(
        relativePath, verbose,
        [&](INT result){
            printf("The total number of type constrained relations is %ld.\n", result);
        }
    );

	// input_file = fopen((inPath + relativePath).c_str(), "r");

	// if (input_file == nullptr) {
	// 	std::cout << '`' << inPath << relativePath << '`' << " does not exist" << std::endl;
	// 	throw std::invalid_argument("File does not exist");
	// }

	// fscanf(input_file, "%ld", total); // Reading number of train samples

    // if (verbose) {
    //     printf("The total number of %s triples is %ld.\n", getSubsetTypeName(subsetType).c_str(), *total);
    // }

    // return input_file;
}
