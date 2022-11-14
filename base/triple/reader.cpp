#include "../Setting.h"
#include "../filters/main.h"

#include "main.h"
#include "reader.h"
#include "TripleIndex.h"

// INT* current_triple_id = new INT(0);

TripleIds readTriples(FILE* input_file, bool enable_filters, Triple* tripleList, TripleIndex* tripleIndex) {
    INT* current_internal_entity_id = new INT(0);
    INT* current_internal_relation_id = new INT(0);

	// Triple* trainList = (Triple *)calloc(trainTotal, sizeof(Triple));

    INT j = 0;

	for (INT i = 0; i < trainTotal; i++) { // Reading train samples
        INT h, r, t;

		fscanf(input_file, "%ld", &h);
		fscanf(input_file, "%ld", &t);
		fscanf(input_file, "%ld", &r);

        // Triple triple = Triple(h, r, t);

        if (!enable_filters || isAcceptableTriple(h, r, t)) {
            // cout << h << " -" << r << "-> " << t << endl;
            if (enable_filters) {
                tripleList[j].h = external_to_internal_id(h, current_internal_entity_id, &external_to_internal_entity_id, &internal_to_external_entity_id);
                tripleList[j].t = external_to_internal_id(t, current_internal_entity_id, &external_to_internal_entity_id, &internal_to_external_entity_id);
                tripleList[j].r = external_to_internal_id(r, current_internal_relation_id, &external_to_internal_relation_id, &internal_to_external_relation_id);

                // trainList[j].print();
            } else {
                tripleList[j].h = h;
                tripleList[j].t = t;
                tripleList[j].r = r;
            }

            tripleIndex->add(tripleList[j]);

            j++;
        }

        // std::cout << "Current train triple: " << trainList[i].as_filterable_string() << "; matches an exclusion pattern: " << doesMatchSomeFilterPatterns(exclusionFilterPatterns, trainList[i]) << endl;
	}

    TripleIds tripleIds;

    tripleIds.last_entity = *current_internal_entity_id;
    tripleIds.last_relation = *current_internal_relation_id;
    tripleIds.last_triple = j;

    return tripleIds;
}
