#include <fstream>
#include <vector>
#include <unordered_set>

#include "main.h"
#include "../../Reader.h"

using namespace std;

vector<vector<Triple>> symmetricTriples;
int nSymmetricTriples = 0;

void separateSymmetricTriples(bool verbose = false) {
    if (verbose) {
		cout << "Separating symmetric triples..." << endl;
    }

    ifstream in_file(inPath + "patterns/symmetric.txt");

    int relation;
    // int nSymmetricRelations = 0;

    // vector<int> symmetricRelations;
    unordered_set<int> symmetricRelations;

    while (in_file >> relation) {
        printf("Read relation %d.\n", relation);

        symmetricRelations.insert(relation);
        // nSymmetricRelations += 1;
    }
    
    printf("Number of symmetric relations = %d.\n", (int)symmetricRelations.size());

    // in_file.clear();
    // in_file.seekg(0);

    // int* symmetricRelations = (int*)calloc(nSymmetricRelations, sizeof(int));
    // int currentRelationIndex = 0;

    // while (in_file >> symmetricRelations[currentRelationIndex++]);

    in_file.close();

    std::cout << "Symmetric triples" << std::endl;

    // Triple** symmetricTriples

	for (INT i = 0; i < trainTotal; i++) { // Reading train samples
        Triple triple = trainList[i];

        // for (int j = 0; j < nSymmetricRelations; j++) {
        // for(int symmetricRelation: symmetricRelations) {
            // if (symmetricRelations[j] == triple.r) {
        // if (symmetricRelation == triple.r) {
        if (symmetricRelations.find(triple.r) != symmetricRelations.end()) {
            // std::vector<Triple> pair = {triple};

            // SymmetricPatternInstance* patternInstance = new SymmetricPatternInstance;

            // patternInstance->triples.push_back(triple);
            // patternInstance->observedTripleIndices.insert(0);

            // Triple* mirroredTriple = (Triple*)malloc(sizeof(Triple));

            // mirroredTriple->r = triple.r;
            // mirroredTriple->h = triple.t;
            // mirroredTriple->t = triple.h;

            // patternInstance->triples.push_back(*mirroredTriple);
            
            SymmetricPatternInstance* patternInstance = new SymmetricPatternInstance(triple, Triple(triple.t, triple.r, triple.h));
            (new SymmetricPatternInstance(Triple(triple.t, triple.r, triple.h), triple, false))->print();

            // patternInstance->triples[0].print();
            // patternInstance->triples[1].print();

            patternInstance->print();

            symmetricTriples.push_back(patternInstance->triples);
            nSymmetricTriples++;
        }
        // }
	}

    std::cout << std::endl << std::endl;
    
    for (std::vector<Triple> pair: symmetricTriples) {
        for (Triple triple: pair) {
            triple.print();
        }
    }
}

