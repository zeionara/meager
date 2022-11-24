#include "../Setting.h"
#include "../filters/main.h"

#include "main.h"
#include "reader.h"
#include "TripleIndex.h"
#include "../utils/main.h"

// INT* current_triple_id = new INT(0);

TripleIds readTriples(File* file, bool enable_filters, Triple* tripleList, TripleIndex* tripleIndex, INT start_internal_entity_id, INT start_internal_relation_id) {
    // cout << "start reading triples" << endl;
    INT* current_internal_entity_id = new INT(start_internal_entity_id < 0 ? 0 : start_internal_entity_id);
    INT* current_internal_relation_id = new INT(start_internal_relation_id < 0 ? 0 : start_internal_relation_id);
    // cout << "continue reading triples" << endl;

	// Triple* trainList = (Triple *)calloc(trainTotal, sizeof(Triple));

    INT j = 0;
    // cout << file->length << endl;
    // cout << "start loop" << endl;

	for (INT i = 0; i < file->length; i++) { // Reading train samples
        INT h, r, t;


		fscanf(file->file, "%ld", &h);
		fscanf(file->file, "%ld", &t);
		fscanf(file->file, "%ld", &r);

        // cout << "scanned" << h << "|" << t << "|" << r << endl;

        // Triple triple = Triple(h, r, t);

        // if (i == 0)
        //     cout << h << " -" << r << "-> " << t << endl;

        if (!enable_filters || isAcceptableTriple(h, r, t)) {
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

            // cout << "saving" << endl;

            tripleIndex->add(tripleList[j]);

            // cout << "saved" << endl;

            j++;

            // cout << j << endl;
        }

        // std::cout << "Current train triple: " << trainList[i].as_filterable_string() << "; matches an exclusion pattern: " << doesMatchSomeFilterPatterns(exclusionFilterPatterns, trainList[i]) << endl;
	}
    // cout << "scanned all" << endl;

    TripleIds tripleIds;

    tripleIds.last_entity = *current_internal_entity_id;
    tripleIds.last_relation = *current_internal_relation_id;
    tripleIds.last_triple = j;

    return tripleIds;
}
