#include <algorithm>
#include "main.h"
#include "TripleIndex.h"

vector<INT> internal_to_external_entity_id;
vector<INT> internal_to_external_relation_id;

unordered_map<INT, INT> external_to_internal_entity_id;
unordered_map<INT, INT> external_to_internal_relation_id;

string getPluralTripleComponentName(TripleComponent component) {
    switch (component) {
        case entity:
            return "entities";
        case ::TripleComponent::relation:
            return "relations";
        default:
            return "unknown";
    }
}

Frequencies* dropDuplicates(TripleLists* lists, INT nEntities, INT nRelations) {
    INT nUniqueTriples = 1;

    Frequencies* frequencies = new Frequencies(nEntities, nRelations);

	std::sort(lists->main, lists->main + lists->length, Triple::cmp_head);

	lists->head[0] = lists->tail[0] = lists->relation[0] = lists->main[0];

	frequencies->entity[lists->main[0].t] += 1;
	frequencies->entity[lists->main[0].h] += 1;
	frequencies->relation[lists->main[0].r] += 1;

	for (INT i = 1; i < lists->length; i++) // Remove duplicated triples
		if (
            lists->main[i].h != lists->main[i - 1].h ||
		    lists->main[i].r != lists->main[i - 1].r ||
		    lists->main[i].t != lists->main[i - 1].t
        ) {
			lists->head[nUniqueTriples] = lists->tail[nUniqueTriples] = lists->relation[nUniqueTriples] = lists->main[nUniqueTriples] = lists->main[i];
			nUniqueTriples++;
			frequencies->entity[lists->main[i].t]++;
			frequencies->entity[lists->main[i].h]++;
			frequencies->relation[lists->main[i].r]++;
		}

	std::sort(lists->head, lists->head + trainTotal, Triple::cmp_head);
	std::sort(lists->tail, lists->tail + trainTotal, Triple::cmp_tail);
	std::sort(lists->relation, lists->relation + trainTotal, Triple::cmp_rel);

    printf("The total of UNIQUE train triples is %ld.\n", nUniqueTriples);

    lists->length = nUniqueTriples;

    return frequencies;
}

INT external_to_internal_id(INT external_id, INT* internal_id, unordered_map<INT, INT>* external_to_internal, vector<INT>* internal_to_external) {
    auto iterator = external_to_internal->find(external_id);

    // cout << "Translating external id " << external_id << " ..." << endl;

    if (iterator == external_to_internal->end()) {
        (*external_to_internal)[external_id] = *internal_id;
        internal_to_external->push_back(external_id);
        
        (*internal_id)++;

        // cout << "Cannot find mapping for external id " << external_id << " - it will be " << (*internal_id) - 1 << endl;

        return (*internal_id) - 1;
    }

    // cout << "Found mapping for external id " << external_id << " - it is " << iterator->second << endl;

    return iterator->second;
}

BoundaryCollection* findBoundaries(TripleLists* lists, Frequencies* frequencies) {
	// lefHead = (INT *)calloc(entityTotal, sizeof(INT));
	// rigHead = (INT *)calloc(entityTotal, sizeof(INT));
	// lefTail = (INT *)calloc(entityTotal, sizeof(INT));
	// rigTail = (INT *)calloc(entityTotal, sizeof(INT));
	// lefRel = (INT *)calloc(entityTotal, sizeof(INT));
	// rigRel = (INT *)calloc(entityTotal, sizeof(INT));

	// memset(rigHead, -1, sizeof(INT) * entityTotal);
	// memset(rigTail, -1, sizeof(INT) * entityTotal);
	// memset(rigRel, -1, sizeof(INT) * entityTotal);
    
    BoundaryCollection* boundaries = new BoundaryCollection(frequencies);

	for (INT i = 1; i < lists->length; i++) { // For each element in any triple position found a (closed) interval in the list of sorted triples in which entries with this element occur
        boundaries->head->update(lists->head, i);
        boundaries->tail->update(lists->tail, i);
        boundaries->relation->update(lists->relation, i);
	}

    boundaries->head->finalize(lists->head, lists->length);
    boundaries->tail->finalize(lists->tail, lists->length);
    boundaries->relation->finalize(lists->relation, lists->length);

    return boundaries;
}

