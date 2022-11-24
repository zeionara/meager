#ifndef TRIPLE_TRIPLE_COMPONENT_ENCODER_H
#define TRIPLE_TRIPLE_COMPONENT_ENCODER_H

#include <unordered_map>
#include <vector>

#include "../Setting.h"

using namespace std;

struct TripleComponentEncoder {

    unordered_map<INT, INT> encodingMap;
    vector<INT> decodingMap;

    INT nEncodedValues = 0;

    INT encode(INT external_id) {
        auto iterator = encodingMap.find(external_id);

        // cout << "Translating external id " << external_id << " ..." << endl;

        if (iterator == encodingMap.end()) {
            encodingMap[external_id] = nEncodedValues;
            decodingMap.push_back(external_id);
            
            return nEncodedValues++;
            // return (*internal_id) - 1;
        }

        return iterator->second;
    }
};

#endif
