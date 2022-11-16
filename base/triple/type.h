#ifndef TRIPLE_RELATION_TYPE_H
#define TRIPLE_RELATION_TYPE_H
#include "main.h"
#include "TripleIndex.h"
#include "reader.h"

#include "../patterns/none/reader.h"
#include "../patterns/symmetric/reader.h"
#include "../patterns/inverse/reader.h"

struct AllowedTripleElements {
    INT* items;
    INT length;

    AllowedTripleElements(FILE* file) {
        INT length;

        fscanf(file, "%ld", &length);

        // cout << "Found " << length << " items" << endl;

        INT* items = (INT*)calloc(length, sizeof(INT));

        for (INT j = 0; j < length; j++) {
            fscanf(file, "%ld", &items[j]);
        }
        std::sort(items, items + length);

        this->items = items;
        this->length = length;
    }

    bool contains(INT value) {
        for (INT i = 0; i < this->length; i++) {
            if (this->items[i] == value) {
                return true;
            }
        }
        return false;
    }
};

struct RelationType {
    AllowedTripleElements* heads;
    AllowedTripleElements* tails;
    INT relation;

    RelationType(FILE* file) {
        INT headRelation;
        fscanf(file, "%ld", &headRelation);

        // cout << "Handling heads for relation " << headRelation << endl;

        heads = new AllowedTripleElements(file);

        INT tailRelation;
        fscanf(file, "%ld", &tailRelation);

        if (tailRelation != headRelation) {
            throw "Relation type can be created only for one relation";
        }

        // cout << "Handling tails for relation " << headRelation << endl;

        tails = new AllowedTripleElements(file);

        this->relation = headRelation;
    }
};

struct RelationTypes {
    RelationType** relations;
    INT length;

    RelationTypes(bool verbose = false) {
        File* file = readNumberOfTypeConstrainedRelations(verbose);

        FILE* f_type = file->file;
        INT nTypeConstrainedRelations = file->length;

        relations = (RelationType**)calloc(file->length, sizeof(RelationType*));

        this->length = file->length;

        for (INT i = 0; i < file->length; i++) {
            relations[i] = new RelationType(file->file);
        }

        fclose(f_type);
    }

    RelationType* get(INT relation) {
        for (INT i = 0; i < this->length; i++) {
            if (this->relations[i]->relation == relation) {
                return this->relations[i];
            }
        }
        return nullptr;
    }
};

#endif
