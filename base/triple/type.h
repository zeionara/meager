#ifndef TRIPLE_RELATION_TYPE_H
#define TRIPLE_RELATION_TYPE_H
#include "main.h"
#include "TripleIndex.h"
#include "reader.h"

#include "../patterns/none/reader.h"
#include "../patterns/symmetric/reader.h"
#include "../patterns/inverse/reader.h"

template <typename T>
struct AllowedTripleElements {
    INT* items;
    INT length;

    AllowedTripleElements(RelationTypeContents<T>* contents) {
        this->items = contents->entities;
        this->length = contents->length;

        std::sort(items, items + length);
    }

    bool contains(INT value) {
        for (INT i = 0; i < this->length; i++) {
            if (this->items[i] == value) {
                return true;
            }
        }
        return false;
    }

    void encode(TripleComponentEncoder<INT>* encoder) {
        INT j = 0;
        for (INT i = 0; i < this->length; i++) {
            if (encoder->contains(this->items[i])) {
                items[j++] = encoder->encode(this->items[i]);
            }
        }
        length = j;
    }
};

template <typename T>
struct RelationType {
    AllowedTripleElements<T>* heads;
    AllowedTripleElements<T>* tails;
    INT relation;

    RelationType(RelationTypeContents<T>* headContents, RelationTypeContents<T>* tailContents) {

        // cout << "Handling heads for relation " << headRelation << endl;

        heads = new AllowedTripleElements<T>(headContents);

        if (headContents->relation != tailContents->relation) {
            throw "Relation type can be created only for one relation";
        }

        // cout << "Handling tails for relation " << headRelation << endl;

        tails = new AllowedTripleElements<T>(tailContents);

        this->relation = headContents->relation;
    }

    void encode(TripleEncoder<INT>* encoder) {
        relation = encoder->relation->encode(relation);
        heads->encode(encoder->entity);
        tails->encode(encoder->entity);
    }
};

template <typename T>
struct RelationTypes {
    RelationType<T>** relations;
    INT length;

    RelationTypes(bool verbose = false) {
        File* file = readNumberOfTypeConstrainedRelations(verbose);

        FILE* f_type = file->file;
        INT nTypeConstrainedRelations = file->length;

        relations = (RelationType<T>**)calloc(file->length, sizeof(RelationType<T>*));

        this->length = file->length;

        // for (INT i = 0; i < file->length; i++) {
        //     relations[i] = new RelationType(file->file);
        // }

        fclose(f_type);
    }

    RelationTypes(bool enableFilters, TripleEncoder<T>* encoder, CorpusReader<T>* reader, bool verbose = false) {
        RelationTypesContents<T>* contents = reader->readRelationTypesContents(verbose);

        relations = (RelationType<T>**)calloc(contents->length, sizeof(RelationType<T>*));

        this->length = contents->length;

        INT j = 0;
        for (INT i = 0; i < contents->length; i += 2) {
            RelationType<T>* relation = new RelationType<T>(contents->relations[i], contents->relations[i + 1]);
            if (!enableFilters || encoder->relation->contains(relation->relation)) {
                if (enableFilters) {
                    relation->encode(encoder);
                }
                relations[j++] = relation;
            }
        }
        length = j;
    }

    RelationType<T>* get(INT relation) {
        for (INT i = 0; i < this->length; i++) {
            if (this->relations[i]->relation == relation) {
                return this->relations[i];
            }
        }
        return nullptr;
    }
};

#endif
