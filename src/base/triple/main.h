#ifndef TRIPLE_MAIN_H
#define TRIPLE_MAIN_H

#include "../types.h"

#include <cstring>
#include <functional>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>

namespace meager::main::triple {

    enum class Component {
        head,
        relation,
        tail
    };

    enum class ComponentType {
        entity,
        relation
    };

    Component decodeComponent(std::string name);

}


#endif
