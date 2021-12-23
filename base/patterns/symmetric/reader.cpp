#include <fstream>
#include <vector>

#include "../../Reader.h"

std::vector<std::vector<Triple>> symmetricTriples;
int nSymmetricTriples = 0;

void separateSymmetricTriples(bool verbose = false) {
    // FILE *input_file;

    if (verbose) {
		std::cout << "Separating symmetric triples..." << std::endl;
    }

    // input_file = fopen((inPath + "patterns/symmetric.txt").c_str(), "r");

	// if (input_file == nullptr) {
	// 	std::cout << '`' << inPath << "patterns/symmetric.txt" << '`' << " does not exist" << std::endl;
	// 	return;
	// }

    // int tmp;
    // int rel; 
	// tmp = fscanf(input_file, "%d", &rel);
    // if (verbose) {
    //     printf("First relation is %d.\n", rel);
    // }
       
    std::ifstream in_file(inPath + "patterns/symmetric.txt");

	// if (in_file == nullptr) {
	// 	std::cout << '`' << inPath << "train2id.txt" << '`' << " does not exist" << std::endl;
	// 	return;
	// }

    int relation;
    int nSymmetricRelations = 0;
    while (in_file >> relation) {
        // std::cout << "Read a relation" << std::endl;
        printf("Read relation %d.\n", relation);

        nSymmetricRelations += 1;
    }
    
    printf("Number of symmetric relations = %d.\n", nSymmetricRelations);

    in_file.clear();
    in_file.seekg(0);

    int* symmetricRelations = (int*)calloc(nSymmetricRelations, sizeof(int));
    int currentRelationIndex = 0;

    while (in_file >> symmetricRelations[currentRelationIndex++]);

    in_file.close();

    std::cout << "Symmetric triples" << std::endl;

    // Triple** symmetricTriples

	for (INT i = 0; i < trainTotal; i++) { // Reading train samples
        Triple triple = trainList[i];

        for (int j = 0; j < nSymmetricRelations; j++) {
            if (symmetricRelations[j] == triple.r) {
                std::vector<Triple> pair = {triple};

                Triple* mirroredTriple = (Triple*)malloc(sizeof(Triple));

                mirroredTriple->r = triple.r;
                mirroredTriple->h = triple.t;
                mirroredTriple->t = triple.h;

                pair.push_back(*mirroredTriple);

                // Triple* pair = (Triple *)calloc(2, sizeof(Triple)); // {triple, *mirroredTriple}; // 

                // pair[1].r = triple.r;
                // pair[1].h = triple.t;
                // pair[1].t = triple.h;

                // memcpy(&pair[0], &triple, sizeof(Triple));
                // memcpy(&pair[1], mirroredTriple, sizeof(Triple));

                // free(mirroredTriple);
                pair[0].print();
                pair[1].print();

                symmetricTriples.push_back(pair);
                // symmetricTriples.push_back(&trainList[i]);
                nSymmetricTriples++;
            }
        }

        // tmp = fscanf(input_file, "%ld", &trainList[i].h);
		// tmp = fscanf(input_file, "%ld", &trainList[i].t);
		// tmp = fscanf(input_file, "%ld", &trainList[i].r);
	}

    std::cout << std::endl << std::endl;

    // for (int i = 0; i < nSymmetricTriples; i++) {
    //     symmetricTriples[i][0].print();
    //     symmetricTriples[i][1].print();
    // }
    
    for (std::vector<Triple> pair: symmetricTriples) {
        // printf("Number of triples per symmetric pattern occurrence = %ld\n", (long)(sizeof pair / sizeof pair[0])) 
        for (Triple triple: pair) {
            triple.print();
        }
        // pair[0].print();
        // pair[1].print();
    }

    // std::string line;
    // while (std::getline(in_file, line))
    // {
    //     std::istringstream iss(line);
    //     int a, b;
    //     if (!(iss >> a >> b)) { break; } // error

    //     // process pair (a,b)
    // }

    // for(std::string line; getline(in_file, line);) {
    //     std::cout << line;
    // }
}

