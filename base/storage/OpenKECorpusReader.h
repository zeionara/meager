#ifndef STORAGE_OPENKE_CORPUS_READER_H
#define STORAGE_OPENKE_CORPUS_READER_H

#include <string>

#include "CorpusReader.h"

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
        File* file = readNumberOfTriples(path + (subsetType == train ? TRAIN_FILENAME : subsetType == test ? TEST_FILENAME : VALID_FILENAME), verbose);
        
        TripleList* triples = new TripleList(file->length, tripleElement);
        Triple* items = triples->items;

        INT j = 0;

        cout << "For each of " << file->length << " triples" << endl;
        // cout << "Filter including empty: " << filter->including->empty << endl;
        // cout << "Filter excluding empty: " << filter->excluding->empty << endl;

        for (INT i = 0; i < file->length; i++) { // Reading train samples
            INT h, r, t;

            fscanf(file->file, "%ld", &h);
            fscanf(file->file, "%ld", &t);
            fscanf(file->file, "%ld", &r);

            // cout << "Filter allows: " << filter->allows(Triple(h, r, t)) << endl;

            if (!enableFilters || filter->allows(Triple(h, r, t))) {
                if (enableFilters) {
                    items[j].h = encoder->entity->encode(h);
                    items[j].t = encoder->entity->encode(t);
                    items[j].r = encoder->relation->encode(r);
                    // cout << "Encoded " << r << endl;
                } else {
                    items[j].h = h;
                    items[j].t = t;
                    items[j].r = r;
                }

                tripleIndex->add(items[j]);

                j++;
            }
        }

        if (enableFilters) {
            cout << "Current entity id in encoder = " << encoder->entity->nEncodedValues << endl;
        }

        cout << "STOP" << endl;

        triples->length = j;

        return triples;
    }

    vector<regex> readFilterPatterns(bool excluding = false, bool verbose = false, bool drop_duplicates = true) {
        if (verbose) {
            cout << "Reading " << path << " triple patterns..." << endl;
        }

        ifstream in_file(path + (excluding ? EXCLUDING_FILTERS_FILENAME : INCLUDING_FILTERS_FILENAME));

        if (!in_file.good()) {
            throw invalid_argument("Cannot find file " + path + " on disk");
        }

        vector<regex> patterns;

        for (string line; getline(in_file, line);) {
            if (drop_duplicates) {
                unordered_set<string> seenPatterns;

                if (seenPatterns.find(line) == seenPatterns.end()) {
                    seenPatterns.insert(line);
                    regex line_regex(line);
                    patterns.push_back(line_regex); 
                    cout << "Read filtering pattern " << line << endl;
                }
            } else {
                regex line_regex(line);
                patterns.push_back(line_regex); 
            }
        }

        return patterns;
    }

    INT readVocabularySize(TripleComponent tripleComponent, bool verbose = false) {
        File* file = readNumberOfElements(path + (tripleComponent == entity ? ENTITIES_FILENAME : RELATIONS_FILENAME), verbose);
        file->close();
        return file->length;
    }

    RelationTypesContents<INT>* readRelationTypesContents(bool verbose = false) {
        File* file = readNumberOfTypeConstrainedRelations(path + TYPE_FILENAME, verbose);

        RelationTypeContents<INT>** relations = (RelationTypeContents<INT>**)calloc(file->length * 2, sizeof(RelationTypeContents<INT>*));

        for (INT i = 0; i < file->length * 2; i++) {
            INT relation;

            fscanf(file->file, "%ld", &relation);

            INT length;

            fscanf(file->file, "%ld", &length);

            INT* items = (INT*)calloc(length, sizeof(INT));

            for (INT j = 0; j < length; j++) {
                fscanf(file->file, "%ld", &items[j]);
            }

            relations[i] = new RelationTypeContents<INT>(items, length, relation);
        }

        return new RelationTypesContents<INT>(relations, file->length * 2);
    }

    // void separateInverseTriples(string path, Triple* triples, INT nTriples, TripleIndex* index, bool verbose, bool drop_duplicates, bool enable_filters) {
    BinaryPatternRelationMap<INT>* readBinaryPatterns(Pattern pattern, TripleEncoder<INT>* encoder, bool enableFilters, bool verbose) {
        string relativePath;

        switch (pattern) {
            case inverse:
                relativePath = INVERSE_PATTERNS_FILENAME;
                break;
            default:
                cout << "Pattern is not binary" << endl;
                throw "Pattern is not binary";
        }

        ifstream in_file(path + relativePath);

        INT firstRelation, secondRelation;

        unordered_map<INT, INT> firstRelationToSecond;
        unordered_map<INT, INT> secondRelationToFirst;

        while (in_file >> firstRelation >> secondRelation) {
            if (verbose) {
                printf("Read relations %ld and %ld.\n", firstRelation, secondRelation);
            }

            firstRelationToSecond[firstRelation] = enableFilters ? encoder->relation->encode(secondRelation) : secondRelation;
            secondRelationToFirst[secondRelation] = enableFilters ? encoder->relation->encode(firstRelation) : firstRelation;
        }
        
        if (verbose) {
            printf("Number of relations in binary pattern = %d.\n", (int)firstRelationToSecond.size());
        }

        in_file.close();

        return new BinaryPatternRelationMap<INT>(firstRelationToSecond, secondRelationToFirst);
    }

    UnaryPatternRelationSet<INT>* readUnaryPatterns(Pattern pattern, TripleEncoder<INT>* encoder, bool enableFilters, bool verbose) {
        string relativePath;

        switch (pattern) {
            case symmetric:
                relativePath = SYMMETRIC_PATTERNS_FILENAME;
                break;
            default:
                cout << "Pattern is not unary" << endl;
                throw "Pattern is not unary";
        }

        ifstream in_file(path + relativePath);

        INT relation;
        unordered_set<INT> relations;

        while (in_file >> relation) {
            if (verbose) {
                printf("Read relation %ld.\n", relation);
            }

            relations.insert(enableFilters ? encoder->relation->encode(relation) : relation);
        }
        
        if (verbose) {
            printf("Number of symmetric relations = %d.\n", (int)relations.size());
        }

        in_file.close();

        if (verbose) {
            cout << "Symmetric triples" << endl;
        }

        return new UnaryPatternRelationSet<INT>(relations);
    }
};

#endif
