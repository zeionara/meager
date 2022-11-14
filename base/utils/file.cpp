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


void readNumberOfElements(TripleComponent component, bool verbose) {
	FILE *input_file;
	int tmp;
    long* total = component == entity ? &entityTotal : &relationTotal;

    std::string relativePath = getInputPath(idMapping, component);
    std::string componentName = component == entity ? "entities" : "relations";

	input_file = fopen((inPath + relativePath).c_str(), "r");

	if (input_file == nullptr) {
	 	std::cout << '`' << inPath << relativePath << '`' << " does not exist" << std::endl;
	 	return;
	}

	tmp = fscanf(input_file, "%ld", total);

    if (verbose) {
        printf("The total number of %s is %ld.\n", getPluralTripleComponentName(component).c_str(), *total);
    }

	fclose(input_file);
}

FILE* readNumberOfTriples(SubsetType subsetType, bool verbose) {
    FILE *input_file;
	int tmp;
    long* total = subsetType == train ? &trainTotal : subsetType == test ? &testTotal : &validTotal;

    std::string relativePath = getInputPath(triples, subsetType);

	input_file = fopen((inPath + relativePath).c_str(), "r");

	if (input_file == nullptr) {
		std::cout << '`' << inPath << relativePath << '`' << " does not exist" << std::endl;
		throw std::invalid_argument("File does not exist");
	}

	fscanf(input_file, "%ld", total); // Reading number of train samples

    if (verbose) {
        printf("The total number of %s triples is %ld.\n", getSubsetTypeName(subsetType).c_str(), *total);
    }

    return input_file;
}
