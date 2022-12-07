#ifndef STORAGE_OPENKE_CORPUS_READER_H
#define STORAGE_OPENKE_CORPUS_READER_H

#include <string>
#include <fstream>

#include "../filters/TripleFilter.h"

#include "CorpusReader.h"

#define getLine getline
#define unorderedSet unordered_set
#define pushBack push_back
#define invalidArgument invalid_argument

using namespace std;

string const TRAIN_FILENAME = "train2id.txt";
string const TEST_FILENAME = "test2id.txt";
string const VALID_FILENAME = "valid2id.txt";

string const TYPE_FILENAME = "type_constrain.txt";

string const ENTITIES_FILENAME = "entity2id.txt";
string const RELATIONS_FILENAME = "relation2id.txt";

string const INCLUDING_FILTERS_FILENAME = "filters/including.txt";
string const EXCLUDING_FILTERS_FILENAME = "filters/excluding.txt";

string const INVERSE_PATTERNS_FILENAME = "patterns/inverse.txt";
string const SYMMETRIC_PATTERNS_FILENAME = "patterns/symmetric.txt";

struct OpenKECorpusReader: CorpusReader<INT> {
    string path;

    OpenKECorpusReader(string path) {
        this->path = path;
    }

    TripleList* readTriples(SubsetType subsetType, TripleIndex* tripleIndex, TripleElement tripleElement, TripleFilter<INT>* filter, TripleEncoder<INT>* encoder, bool enableFilters, bool verbose) {
        File file = File(
            path + (
                subsetType == SubsetType::train ? TRAIN_FILENAME :
                subsetType == SubsetType::test ? TEST_FILENAME :
                subsetType == SubsetType::valid ? VALID_FILENAME :
                throw invalidArgument("Unknown subset type")
            ), verbose
        );

        TripleList* triples = new TripleList(file.getLength(), tripleElement);
        Triple* items = triples->items;

        INT j = 0;

        if (verbose) {
            cout << "For each of " << file.getLength() << " triples" << endl;
        }

        for (INT i = 0; i < file.getLength(); i++) { // Reading train samples
        // while (file.good()) {
            INT h = 0, r = 0, t = 0;

            file.stream >> h >> t >> r;

            // fscanf(file->file, "%ld", &h);
            // fscanf(file->file, "%ld", &t);
            // fscanf(file->file, "%ld", &r);

            if (!enableFilters || filter->allows(Triple(h, r, t))) {
                if (enableFilters) {
                    items[j].h = encoder->entity->encode(h);
                    items[j].t = encoder->entity->encode(t);
                    items[j].r = encoder->relation->encode(r);
                } else {
                    items[j].h = h;
                    items[j].t = t;
                    items[j].r = r;
                }

                tripleIndex->add(items[j]);

                j++;
            }
        }

        if (enableFilters && verbose) {
            cout << "Current entity id in encoder = " << encoder->entity->nEncodedValues << endl;
        }

        triples->length = j;

        return triples;
    }

    vector<regex> readFilterPatterns(bool excluding = false, bool verbose = false, bool dropDuplicates = true) {
        if (verbose) {
            cout << "started reading filter patterns" << endl;
        }

        ifstream inFile(path + (excluding ? EXCLUDING_FILTERS_FILENAME : INCLUDING_FILTERS_FILENAME));

        if (!inFile.good()) {
            throw invalidArgument("cannot find file with filter on disk");
        }

        vector<regex> patterns;

        for (string line; getLine(inFile, line);) {
            if (dropDuplicates) {
                unorderedSet<string> seenPatterns;

                if (seenPatterns.find(line) == seenPatterns.end()) {
                    seenPatterns.insert(line);
                    regex lineRegex(line);
                    patterns.pushBack(lineRegex);

                    if (verbose) {
                        cout << "read filter pattern " << line << endl;
                    }
                }
            } else {
                regex lineRegex(line);
                patterns.pushBack(lineRegex);
            }
        }

        if (verbose) {
            cout << "finished reading filter patterns" << endl;
        }

        return patterns;
    }

    INT readVocabularySize(TripleComponent tripleComponent, bool verbose = false) {
        File* file = new File(path + (tripleComponent == entity ? ENTITIES_FILENAME : RELATIONS_FILENAME), verbose);
        INT length = file->getLength();
        file->close();
        return length;
    }

    RelationTypesContents<INT>* readRelationTypesContents(bool verbose = false) {
        File file = File(path + TYPE_FILENAME, verbose);

        // cout << "ff" << endl;

        RelationTypeContents<INT>** relations = (RelationTypeContents<INT>**)calloc(file.getLength() * 2, sizeof(RelationTypeContents<INT>*));

        // cout << file.getLength() << endl;
        // cout << file.getLength() << endl;
        // cout << "ff" << endl;

        for (INT i = 0; i < file.getLength() * 2; i++) {
            INT relation = 0;

            // cout << file->length << endl;
            // cout << "bar" << endl;
            // cout << file->stream << endl;
            file >> relation;
            // cout << "handling relation " << relation << endl;
            // fscanf(file->file, "%ld", &relation);

            INT length = 0;

            file >> length;
            // cout << "number of elements for relation = " << length << endl;
            // cout << length << endl;
            // fscanf(file->file, "%ld", &length);

            INT* items = (INT*)calloc(length, sizeof(INT));

            for (INT j = 0; j < length; j++) {
                // fscanf(file->file, "%ld", &items[j]);
                file >> items[j];
            }

            relations[i] = new RelationTypeContents<INT>(items, length, relation);
        }

        return new RelationTypesContents<INT>(relations, file.getLength() * 2);
    }

    // void separateInverseTriples(string path, Triple* triples, INT nTriples, TripleIndex* index, bool verbose, bool drop_duplicates, bool enable_filters) {
    BinaryPatternRelationMap<INT>* readBinaryPatterns(Pattern pattern, TripleEncoder<INT>* encoder, bool enableFilters, bool verbose) {
        string relativePath;

        switch (pattern) {
            case inverse:
                relativePath = INVERSE_PATTERNS_FILENAME;
                break;
            default:
                cerr << "Pattern is not binary" << endl;
                throw invalidArgument("Pattern is not binary");
        }

        ifstream inFile(path + relativePath);

        if (!inFile.good()) {
            throw invalidArgument("cannot find file with binary pattern relations list on disk");
        }

        INT firstRelation, secondRelation;

        unordered_map<INT, INT> firstRelationToSecond;
        unordered_map<INT, INT> secondRelationToFirst;

        while (inFile >> firstRelation >> secondRelation) {
            if (verbose) {
                printf("Read relations %ld and %ld.\n", firstRelation, secondRelation);
            }

            firstRelationToSecond[firstRelation] = enableFilters ? encoder->relation->encode(secondRelation) : secondRelation;
            secondRelationToFirst[secondRelation] = enableFilters ? encoder->relation->encode(firstRelation) : firstRelation;
        }

        if (verbose) {
            printf("Number of relations in binary pattern = %d.\n", (int)firstRelationToSecond.size());
        }

        inFile.close();

        return new BinaryPatternRelationMap<INT>(firstRelationToSecond, secondRelationToFirst);
    }

    UnaryPatternRelationSet<INT>* readUnaryPatterns(Pattern pattern, TripleEncoder<INT>* encoder, bool enableFilters, bool verbose) {
        string relativePath;

        switch (pattern) {
            case symmetric:
                relativePath = SYMMETRIC_PATTERNS_FILENAME;
                break;
            default:
                cerr << "Pattern is not unary" << endl;
                throw invalidArgument("Pattern is not unary");
        }

        ifstream inFile(path + relativePath);

        if (!inFile.good()) {
            throw invalidArgument("cannot find file with unary pattern relations list on disk");
        }

        INT relation;
        unordered_set<INT> relations;

        while (inFile >> relation) {
            if (verbose) {
                printf("Read relation %ld.\n", relation);
            }

            relations.insert(enableFilters ? encoder->relation->encode(relation) : relation);
        }

        if (verbose) {
            printf("Number of symmetric relations = %d.\n", (int)relations.size());
        }

        inFile.close();

        if (verbose) {
            cout << "Symmetric triples" << endl;
        }

        return new UnaryPatternRelationSet<INT>(relations);
    }
};

#endif
