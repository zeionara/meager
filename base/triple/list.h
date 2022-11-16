#ifndef TRIPLE_LIST_H
#define TRIPLE_LIST_H
#include "main.h"
#include "TripleIndex.h"
#include "reader.h"

#include "../patterns/none/reader.h"
#include "../patterns/symmetric/reader.h"
#include "../patterns/inverse/reader.h"

struct TripleList {
    Triple* items;
    INT* left;
    INT* right;
    INT (*getTripleComponent)(Triple triple);
    TripleElement element;

    INT length;

    TripleList(INT length, TripleElement element) {
        this->items = (Triple *)calloc(length, sizeof(Triple));
        this->length = length;
        this->element = element;

        switch (element) {
            case head:
                this->getTripleComponent = [](Triple triple){return triple.h;};
                break;
            case rel:
                this->getTripleComponent = [](Triple triple){return triple.r;};
                break;
            case tail:
                this->getTripleComponent = [](Triple triple){return triple.t;};
        }
    }

    void sort(INT nItems) {
        std::sort(
            this->items, this->items + this->length,
            this->element == rel ? Triple::cmp_rel2 : this->element == ::TripleElement::head ? Triple::cmp_head : Triple::cmp_tail
        );
        // cout << this->items[0].h << "|" << this->items[0].r << "|" << this->items[0].t << endl;
        // cout << "Sorting..." << nItems << endl;

        INT* left = (INT *)calloc(nItems, sizeof(INT));
        INT* right = (INT *)calloc(nItems, sizeof(INT));

        memset(left, -1, sizeof(INT) * nItems);
        memset(right, -1, sizeof(INT) * nItems);
        // cout << "Sorting..." << endl;

        for (INT i = 1; i < this->length; i++) { // Get intervals for unique relationships in the test subset
            // cout << "Update " << i << endl;
            this->update(i, left, right);
        }
        // cout << "Sorting..." << endl;

        this->left = left;
        this->right = right;
    }

    void update(INT i, INT* left, INT* right) {
		if (this->getTripleComponent(this->items[i]) != this->getTripleComponent(this->items[i - 1])) { // lef - left boundary of such an interval, rig - right boundary
			right[this->getTripleComponent(this->items[i - 1])] = i - 1;
			left[this->getTripleComponent(this->items[i])] = i;
		}
    }

    void finalize() {
        if (this->length > 0) {
            left[this->getTripleComponent(this->items[0])] = 0;
        }
        if (this->length > 1) {
            right[this->getTripleComponent(this->items[this->length - 1])] = this->length - 1;
        }
    }
};

struct RelationScore {
    REAL* head; 
    REAL* tail;

    INT length;

    // REAL handleEntity(Boundaries* boundaries, Triple* triples, INT entity, void (*incrementScore)(INT relation)) {
    REAL handleEntity(TripleList* list, INT entity, std::function<void(INT)> incrementScore) {
        REAL value = 0;

        for (INT j = list->left[entity] + 1; j <= list->right[entity]; j++)
            if (list->items[j].r != list->items[j - 1].r) // Count number of entities "emitting" each relation // Count (number of unique relations - 1) for triples having head "i"
                incrementScore(list->items[j].r);
        if (list->left[entity] <= list->right[entity])
            incrementScore(list->items[list->left[entity]].r);

        return value;
    }

    RelationScore(TripleList* headList, TripleList* tailList, Frequencies* frequencies) {
        REAL* head = (REAL *)calloc(frequencies->nRelations, sizeof(REAL));
        REAL* tail = (REAL *)calloc(frequencies->nRelations, sizeof(REAL));


        for (INT i = 0; i < frequencies->nEntities; i++) {
            handleEntity(headList, i, [&](INT relation){head[relation] += 1.0;});
            handleEntity(tailList, i, [&](INT relation){tail[relation] += 1.0;});
            // for (INT j = lefHead[i] + 1; j <= rigHead[i]; j++)
            //     if (trainHead[j].r != trainHead[j - 1].r) // Count number of entities "emitting" each relation // Count (number of unique relations - 1) for triples having head "i"
            //         left_mean[trainHead[j].r] += 1.0;
            // if (lefHead[i] <= rigHead[i])
            //     left_mean[trainHead[lefHead[i]].r] += 1.0;

            // for (INT j = lefTail[i] + 1; j <= rigTail[i]; j++)
            //     if (trainTail[j].r != trainTail[j - 1].r) // Same as above but for triples having tail "i"
            //         right_mean[trainTail[j].r] += 1.0;
            // if (lefTail[i] <= rigTail[i])
            //     right_mean[trainTail[lefTail[i]].r] += 1.0;
        }
        for (INT i = 0; i < frequencies->nRelations; i++) {
            // cout << frequencies->relation[i] << endl;
            // cout << head[i] << endl;
            // cout << tail[i] << endl;
            head[i] = frequencies->relation[i] / head[i]; // The greater the value the fewer unique heads connects the relation
            tail[i] = frequencies->relation[i] / tail[i]; // The greater the value the fewer unique tails connects the relation
        }

        this->head = head;
        this->tail = tail;
        this->length = frequencies->nRelations;
    }

    void print() {
        std::cout << "--- " << " relation scores"  << std::endl;
        int i = 0;
        for (INT i = 0; i < this->length; i++) {
            std::cout << "| relation = " << i << ", head score = " << this->head[i] << ", tail score = " << this->tail[i] << std::endl;
        }
        std::cout << "---" << std::endl;
    }
};


struct TrainTripleLists {
    TripleList* main;
    TripleList* head;
    TripleList* relation;
    TripleList* tail;

    INT length;

    TripleIndex* index;
    Frequencies* frequencies;
    RelationScore* relationScore;

    // INT lastEntity;
    // INT lastRelation;
    // INT lastTriple;

    TrainTripleLists(SubsetType subset, bool enable_filters = false, bool verbose = false) {
        File* file = readNumberOfTriples(subset, verbose);

        this->main = new TripleList(file->length, ::TripleElement::head);
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
        Triple* main = this->main->items;

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

        // std::sort(head, head + trainTotal, Triple::cmp_head);
        // std::sort(tail, tail + trainTotal, Triple::cmp_tail);
        // std::sort(relation, relation + trainTotal, Triple::cmp_rel);

        // printf("The total of UNIQUE train triples is %ld.\n", nUniqueTriples);

        this->length = nUniqueTriples;
        this->main->length = nUniqueTriples;
        this->head->length = nUniqueTriples;
        this->tail->length = nUniqueTriples;
        this->relation->length = nUniqueTriples;

        this->frequencies = frequencies;
    }

    void sort() {
        // cout << "Sorting main" << endl;
        this->main->sort(this->frequencies->nEntities);
        // cout << "Sorting head" << endl;
        this->head->sort(this->frequencies->nEntities);
        // cout << "Sorting tail" << endl;
        this->tail->sort(this->frequencies->nEntities);
        // cout << "Sorting relation" << endl;
        this->relation->sort(this->frequencies->nRelations);
    }

    void read(File* file, bool enable_filters = false, bool verbose = false) {
        // File* file = readNumberOfTriples(subset, verbose);
        // cout << "bar" << endl;

        TripleIds tripleIds = readTriples(file, enable_filters, this->main->items, this->index);

        if (verbose) {
            printf("n train triples: %ld", tripleIds.last_triple);
        }

        // cout << "Got triple ids" << endl;

        // cout << "foo" << endl;

        file->close();

        // cout << "foo" << endl;

        // this->lastEntity = tripleIds.last_entity;
        // this->lastRelation = tripleIds.last_relation;
        // this->lastTriple = tripleIds.last_triple;

        if (enable_filters) {
            this->dropDuplicates(tripleIds.last_entity, tripleIds.last_relation);
        } else {
            File* relations = readNumberOfElements(TripleComponent::relation, verbose);
            File* entities = readNumberOfElements(TripleComponent::entity, verbose);

            this->dropDuplicates(entities->length, relations->length);
        }

        // cout << "foo" << endl;

        separateNoneTriples(this->main->items, this->length, verbose, true, enable_filters);
        separateSymmetricTriples(this->main->items, this->length, verbose);
        separateInverseTriples(this->main->items, this->length, this->index, verbose, true, enable_filters);
        
        this->sort();

        // cout << "sorted" << endl;

        this->relationScore = new RelationScore(this->head, this->tail, this->frequencies);
    }
};

struct TestTripleLists {
    TripleList* main;

    INT length;

    TripleIndex* index;

    INT nEntities;
    INT nRelations;

    TestTripleLists(SubsetType subset, INT startInternalEntityId, INT startInternalRelationId, bool enable_filters = false, bool verbose = false) {
        cout << "Before read" << endl;
        File* file = readNumberOfTriples(subset, verbose);

        this->main = new TripleList(file->length, ::TripleElement::rel);

        this->length = file->length;
        this->index = new TripleIndex;
        cout << "Before read" << endl;

        this->read(file, startInternalEntityId, startInternalRelationId, enable_filters, verbose);
    }

    void sort() {
        this->main->sort(this->nRelations);
    }

    void read(File* file, INT startInternalEntityId, INT startInternalRelationId, bool enable_filters = false, bool verbose = false) {
        // cout << "uuu" << endl;
        TripleIds tripleIds = readTriples(file, enable_filters, this->main->items, this->index, startInternalEntityId, startInternalRelationId);
        // cout << "uuu" << endl;

        file->close();

        this->nEntities = tripleIds.last_entity;
        this->nRelations = tripleIds.last_relation;

        this->sort();
    }
};
#endif

