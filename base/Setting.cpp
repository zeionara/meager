#define INT long
#define REAL float
#include <cstring>
#include <cstdio>
#include <string>
#include "Reader.h"

#include "storage/LocalTsvCorpus.h"
#include "global.h"

std::string inPath = "../data/FB15K/"; // By default fb15k corpus is used
std::string outPath = "../data/FB15K/";

extern "C"
void setInPath(char *path, bool as_tsv) { // supported by erlang adapter // TODO: rename to 'initCorpus'
    corpus = new LocalTsvCorpus(path, false);

    // TODO: delete

	INT len = strlen(path);
	inPath = "";

	for (INT i = 0; i < len; i++)
		inPath = inPath + path[i];

    if (!as_tsv)
        printf("corpus path : %s\n", inPath.c_str());
}

extern "C"
void setOutPath(char *path) {
	INT len = strlen(path);
	outPath = "";
	for (INT i = 0; i < len; i++)
		outPath = outPath + path[i];
	printf("Output Files Path : %s\n", outPath.c_str());
}

/*
============================================================
*/

INT workThreads = 1; // By default concurrency is disabled

extern "C"
void setWorkThreads(INT threads) {
	workThreads = threads;
}

extern "C"
INT getWorkThreads() {
	return workThreads;
}

/*
============================================================
*/

// INT relationTotal = 0;
// INT entityTotal = 0;
// INT tripleTotal = 0;
// INT testTotal = 0;
// INT trainTotal = 0;
// INT validTotal = 0;

extern "C"
INT getEntityTotal() {
	return trainList->frequencies->nEntities;
}

extern "C"
INT getRelationTotal() {
	return trainList->frequencies->nRelations;
}

extern "C"
INT getTripleTotal() {
	return trainList->length + testList->length + validList->length;
}

extern "C"
INT getTrainTotal() {
	return trainList->length;
}

extern "C"
INT getTestTotal() {
	return testList->length;
}

extern "C"
INT getValidTotal() {
	return validList->length;
}
/*
============================================================
*/

// INT bernFlag = 0;
// INT crossSamplingFlag = 0;

extern "C"
void setBern(INT con) { // supported by erlang adapter
	// bernFlag = con;
}

extern "C"
void setHeadTailCrossSampling(INT con){
	// crossSamplingFlag = con;
}

