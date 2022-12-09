#include <algorithm>
#include "main.h"
#include "TripleIndex.h"

#define invalidArgument invalid_argument

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

TripleElement decodeTripleElement(string name) {

    if (name == "head") {
        return head;
    }

    if (name == "tail") {
        return tail;
    }

    if (name == "relation") {
        return rel;
    }

    throw invalidArgument("Unknown triple element name: " + name);
}

Frequencies* dropDuplicates(Triple* main, Triple* heads, Triple* tails, Triple* relations, INT length, INT nEntities, INT nRelations) {
    INT nUniqueTriples = 1;

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

    // lists->length = nUniqueTriples;

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

void print_triples(std::string header, Triple* triples, int nTriples) {
    std::cout << header << "\n";
    for (int i = 0; i < nTriples; i++) {
        triples[i].print();
    }
}
