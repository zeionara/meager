#include "Tree.h"

namespace meager::main::evaluation::metric::tree {

    Root::~Root() {
        delete tree;
    }

    Node::~Node() {
        delete value;
    }

}
