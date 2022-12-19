#ifndef TRIPLE_LIST_THICK_WRAPPER_H
#define TRIPLE_LIST_THICK_WRAPPER_H

#include "../Index.h"
// 
// #include "RelationScore.h"
// #include "../../filters/TripleFilter.h"
// #include "List.h"
#include "../Encoder.h"
// #include "../ComponentFrequencies.h"
#include "../relation/Score.h"
// #include "../../patterns/inverse/InversePatternDescription.h"
// #include "../../patterns/symmetric/SymmetricPatternDescription.h"
// #include "../../patterns/none/NonePatternDescription.h"
// 
#include "../pattern/description/Descriptions.h"
// #include "../../patterns/PatternDescriptionTemplates.h"

namespace meager::main::triple::list {

    template <typename T>
    struct ThickWrapper {

        List* content;

        INT length;

        List* head;
        List* relation;
        List* tail;

        Index* index;
        ComponentFrequencies* frequencies;
        relation::Score* relationScore;

        pattern::description::Descriptions<T>* patterns;

        ThickWrapper(
            subset::Type subset, corpus::reader::Corpus<T>* reader, filter::Filter<T>* filter, Encoder<T>* encoder, pattern::description::Templates<T>* patterns,
            bool enableFilters, bool dropPatternDuplicates = true, bool verbose = false
        ) {

            index = new Index();

            this->content = reader->readTriples(subset, index, Component::head, filter, encoder, enableFilters, verbose);
            this->head = new List(content->length, Component::head);
            this->relation = new List(content->length, Component::relation);
            this->tail = new List(content->length, Component::tail);

            this->length = content->length;

            this->read(filter, encoder, reader, patterns, enableFilters, dropPatternDuplicates, verbose);
        }

        void dropDuplicates(INT nEntities, INT nRelations) {
            INT nUniqueTriples = 1;

            Triple* heads = this->head->items;
            Triple* tails = this->tail->items;
            Triple* relations = this->relation->items;
            Triple* main = this->content->items;

            ComponentFrequencies* frequencies = new ComponentFrequencies(nEntities, nRelations);

            std::sort(main, main + length, Triple::compareHeadRelationTail);

            heads[0] = tails[0] = relations[0] = main[0];

            frequencies->entity[main[0].t] += 1;
            frequencies->entity[main[0].h] += 1;
            frequencies->relation[main[0].r] += 1;

            for (INT i = 1; i < length; i++) { // Remove duplicated triples
                // cout << i << endl;
                // cout << main[i].h << endl;
                // cout << main[i].r << endl;
                // cout << main[i].t << endl;
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
            }

            // cout << "FOO" << endl;

            this->length = nUniqueTriples;
            this->content->length = nUniqueTriples;
            this->head->length = nUniqueTriples;
            this->tail->length = nUniqueTriples;
            this->relation->length = nUniqueTriples;

            this->frequencies = frequencies;
        }

        void sort(bool verbose = false) {
            if (verbose) {
                cout << "started sorting content" << endl;
            }

            this->content->sort(this->frequencies->nEntities);

            if (verbose) {
                cout << "finished sorting content" << endl;
            }

            this->head->sort(this->frequencies->nEntities);

            if (verbose) {
                cout << "finished sorting head, started sorting tail" << endl;
            }

            this->tail->sort(this->frequencies->nEntities);

            if (verbose) {
                cout << "finished sorting tail, started sorting relation" << endl;
            }

            this->relation->sort(this->frequencies->nEntities, Triple::compareHeadTailRelation); // Sort by head, then by tail, then by relation

            if (verbose) {
                cout << "finished sorting relation" << endl;
            }
        }

        void read(
            filter::Filter<T>* filter, Encoder<T>* encoder, corpus::reader::Corpus<T>* reader, pattern::description::Templates<T>* patterns,
            bool enableFilters, bool dropPatternDuplicates = true, bool verbose = false
        ) {
            if (verbose) {
                cout << "n train triples before dropping duplicates:" << this->length << ", started reading patterns" << endl;
            }

            this->patterns = new pattern::description::Descriptions<T>(content, patterns, index, encoder, dropPatternDuplicates, enableFilters, verbose);

            if (verbose) {
                cout << "finished reading patterns" << endl;
            }

            if (enableFilters) {
                if (verbose) {
                    cout << "there are " << encoder->entity->nEncodedValues << " entities" << endl;
                }
                this->dropDuplicates(encoder->entity->nEncodedValues, encoder->relation->nEncodedValues);
            } else {
                this->dropDuplicates(reader->readVocabularySize(ComponentType::entity), reader->readVocabularySize(ComponentType::relation));
            }

            if (verbose) {
                cout << "n train triples after dropping duplicates: " << this->length << endl;
            }
            
            this->sort();

            if (verbose) {
                cout << "started making relation score" << endl;
            }

            this->relationScore = new relation::Score(this->head, this->tail, this->frequencies);

            if (verbose) {
                cout << "finished making relation score" << endl;
            }
        }

    };

}

#endif
