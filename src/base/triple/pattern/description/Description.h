#ifndef TRIPLE_PATTERN_DESCRIPTION_DESCRIPTION_H
#define TRIPLE_PATTERN_DESCRIPTION_DESCRIPTION_H

#include "../main.h"

using namespace std;

namespace meager::main::triple::pattern::description {

    struct Description {
        string label;
        Pattern id;
        int nTriplesPerInstance;
        vector<Instance>** instanceSets;

        void initInstanceLists() {
            // instanceSets = (vector<Instance>**)malloc(sizeof(vector<Instance>*) * (nTriplesPerInstance + 1));
            instanceSets = new vector<Instance>*[nTriplesPerInstance + 1];

            for (int i = 0; i <= nTriplesPerInstance; i++) {
                instanceSets[i] = new vector<Instance>;
            }
        }

        void push(Instance instance) {
            for (int j = 0; j <= nTriplesPerInstance; j++) {
                if (j <= instance.observedTripleIndices.size()) {
                   instanceSets[j]->push_back(instance); 
                }
            }
        }

        void tryPush(Instance instance, unordered_set<string> seenInstances, bool dropDuplicates = true) {
            if (dropDuplicates) {
                string direct_pattern_instance_concise_description = instance.getConciseDescription();

                if (seenInstances.find(direct_pattern_instance_concise_description) == seenInstances.end()) {
                    push(instance);
                    seenInstances.insert(direct_pattern_instance_concise_description);
                }
            } else {
                push(instance);
            }
        }

        void printSummary(INT nInstancesTarget = -1) {
            if (nInstancesTarget > -1) {
                cout << "Should collect " << nInstancesTarget << " " << label << " pattern instances" << endl;
            }

            for (int i = 0; i <= nTriplesPerInstance; i++) {
                cout << "Collected " << instanceSets[i]->size() << " " << label << " pattern instances in which there are " << i << " or more observed patterns" << endl;
            }

            cout << endl;
        }

    };

}

#endif
