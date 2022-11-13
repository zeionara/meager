#include "Triple.h"
#include "Utils.h"

// enum InputFileType { idMapping, train, test, valid, typeConstraint };

std::string getInputPath(InputFileType fileType, TripleComponent component) {
    switch (fileType) {
        case train:
            return "train2id.txt";
        case test:
            return "test2id.txt";
        case valid:
            return "valid2id.txt";
        case idMapping:
            switch (component) {
                case entity:
                    return "entity2id.txt";
                case ::TripleComponent::relation:
                    return "relation2id.txt";
            }
        case typeConstraint:
            return "type_constrain.txt";
    }
}

void readNumberOfElements(TripleComponent component, bool verbose) {
	FILE *input_file;
	int tmp;

    std::string relativePath = getInputPath(idMapping, component);
    std::string componentName = component == entity ? "entities" : "relations";

	input_file = fopen((inPath + relativePath).c_str(), "r");

	if (input_file == nullptr) {
	 	std::cout << '`' << inPath << relativePath << '`' << " does not exist" << std::endl;
	 	return;
	}

	tmp = fscanf(input_file, "%ld", &relationTotal);

    if (verbose) {
        printf("The total number of %s is %ld.\n", getPluralTripleComponentName(component).c_str(), relationTotal);
    }

	fclose(input_file);
}
