#ifndef TRIPLE_LIST_THICK_TRIPLE_LIST_WRAPPER_H
#define TRIPLE_LIST_THICK_TRIPLE_LIST_WRAPPER_H

#include "../TripleIndex.h"
#include "../reader.h"

#include "../../patterns/none/reader.h"
#include "../../patterns/symmetric/reader.h"
#include "../../patterns/inverse/reader.h"

#include "RelationScore.h"

struct ThickTripleListWrapper {
    TripleList* content;

    INT length;

    TripleList* head;
    TripleList* relation;
    TripleList* tail;

    TripleIndex* index;
    Frequencies* frequencies;
    RelationScore* relationScore;

    ThickTripleListWrapper(SubsetType subset, bool enable_filters = false, bool verbose = false) {
        File* file = readNumberOfTriples(subset, verbose);

        this->content = new TripleList(file->length, ::TripleElement::head);
        this->head = new TripleList(file->length, ::TripleElement::head);
        this->relation = new TripleList(file->length, rel);
        this->tail = new TripleList(file->length, ::TripleElement::tail);

        this->length = file->length;
        this->index = new TripleIndex();

        this->read(file, enable_filters, verbose);
    }

    ThickTripleListWrapper(string path, bool enable_filters = false, bool verbose = false) {
        File* file = readNumberOfTriples(path, verbose);

        this->content = new TripleList(file->length, ::TripleElement::head);
        this->head = new TripleList(file->length, ::TripleElement::head);
        this->relation = new TripleList(file->length, rel);
        this->tail = new TripleList(file->length, ::TripleElement::tail);

        this->length = file->length;
        this->index = new TripleIndex();

        this->read(file, enable_filters, verbose);
    }

    void dropDuplicates(INT nEntities, INT nRelations) {
        INT nUniqueTriples = 1;

        Triple* heads = this->head->items;
        Triple* tails = this->tail->items;
        Triple* relations = this->relation->items;
        Triple* main = this->content->items;

        Frequencies* frequencies = new Frequencies(nEntities, nRelations);

        std::sort(main, main + length, Triple::cmp_head);

        heads[0] = tails[0] = relations[0] = main[0];

        frequencies->entity[main[0].t] += 1;
        frequencies->entity[main[0].h] += 1;
        frequencies->relation[main[0].r] += 1;

        for (INT i = 1; i < length; i++) // Remove duplicated triples
            if (
                main[i].h != main[i - 1].h ||
                main[i].r != main[i - 1].r ||
                main[i].t != main[i - 1].t
            ) {
                heads[nUniqueTriples] = tails[nUniqueTriples] = relations[nUniqueTriples] = main[nUniqueTriples] = main[i];
                nUniqueTriples++;
                frequencies->entity[main[i].t]++;
                frequencies->entity[main[i].h]++;
                frequencies->relation[main[i].r]++;
            }

        this->length = nUniqueTriples;
        this->content->length = nUniqueTriples;
        this->head->length = nUniqueTriples;
        this->tail->length = nUniqueTriples;
        this->relation->length = nUniqueTriples;

        this->frequencies = frequencies;
    }

    void sort() {
        this->content->sort(this->frequencies->nEntities);
        this->head->sort(this->frequencies->nEntities);

        // for (INT i = 0; i < this->head->length; i++) {
        //     if (this->head->left[i] == -1) {
        //         cout << i << endl;
        //     }
        // }

        // throw "stop";

        this->tail->sort(this->frequencies->nEntities);
        this->relation->sort(this->frequencies->nEntities);
        // this->relation->sort(this->frequencies->nRelations);
    }

    void read(File* file, bool enable_filters = false, bool verbose = false) {
        // cout << "Before reading triples" << endl;
        TripleIds tripleIds = readTriples(file, enable_filters, this->content->items, this->index);
        // cout << "After reading triples" << endl;

        if (verbose) {
            printf("n train triples: %ld", tripleIds.last_triple);
        }

        file->close();

        separateNoneTriples(this->content->items, this->length, verbose, true, enable_filters);
        separateSymmetricTriples(this->content->items, this->length, verbose);
        separateInverseTriples(this->content->items, this->length, this->index, verbose, true, enable_filters);

        // Maybe do this before sampling patterns?
        if (enable_filters) {
            this->dropDuplicates(tripleIds.last_entity, tripleIds.last_relation);
        } else {
            File* relations = readNumberOfElements(TripleComponent::relation, verbose);
            File* entities = readNumberOfElements(TripleComponent::entity, verbose);

            this->dropDuplicates(entities->length, relations->length);
        }
        
        this->sort();

        this->relationScore = new RelationScore(this->head, this->tail, this->frequencies);
    }
};

#endif
