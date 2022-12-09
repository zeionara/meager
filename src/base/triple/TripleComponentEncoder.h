#ifndef TRIPLE_TRIPLE_COMPONENT_ENCODER_H
#define TRIPLE_TRIPLE_COMPONENT_ENCODER_H

#include <unordered_map>
#include <vector>

#include "../types.h"

using namespace std;

namespace meager::main::triple {

    template <typename K>
    struct TripleComponentEncoder {

        unordered_map<K, INT> encodingMap;
        vector<K> decodingMap;

        INT nEncodedValues = 0;

        INT encode(K external_id) {
            auto iterator = encodingMap.find(external_id);

            if (iterator == encodingMap.end()) {
                encodingMap[external_id] = nEncodedValues;
                decodingMap.push_back(external_id);
                
                return nEncodedValues++;
            }

            return iterator->second;
        }

        bool contains(K external_id) {
            auto iterator = encodingMap.find(external_id);

            if (iterator == encodingMap.end()) {
                return false;
            }

            return true;
        }
    };

}

#endif
