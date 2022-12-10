#ifndef TRIPLE_LIST_LIST_H
#define TRIPLE_LIST_LIST_H

#include "../main.h"
#include "../Triple.h"

namespace meager::main::triple::list {

    struct List {
        Triple* items;

        INT length;

        INT* left;
        INT* right;

        INT (*getTripleComponent)(Triple triple);
        Component component;

        List(INT length, Component component) {
            this->items = (Triple *)calloc(length, sizeof(Triple));
            this->length = length;
            this->component = component;

            switch (component) {
                case Component::head:
                    this->getTripleComponent = [](Triple triple){return triple.h;};
                    break;
                case Component::relation:
                    this->getTripleComponent = [](Triple triple){return triple.r;};
                    break;
                case Component::tail:
                    this->getTripleComponent = [](Triple triple){return triple.t;};
            }
        }

        void sort(INT nItems, bool (*compareTriples)(const Triple &, const Triple &) = nullptr) { // number of unique relations or entities depending on this->element
            std::sort(
                this->items, this->items + this->length,
                compareTriples == nullptr ?
                    this->component == Component::relation ? Triple::compareRelationHeadTail :
                    this->component == Component::head ? Triple::compareHeadRelationTail :
                    this->component == Component::tail ? Triple::compareTailRelationHead :
                    throw "Unknown triple component"
                : compareTriples
            );

            INT* left = new INT[nItems]; // (INT *)calloc(nItems, sizeof(INT));
            INT* right = new INT[nItems]; // (INT *)calloc(nItems, sizeof(INT));

            memset(left, -1, sizeof(INT) * nItems);
            memset(right, -1, sizeof(INT) * nItems);

            for (INT i = 1; i < this->length; i++) { // get intervals for unique relationships in the test subset
                this->update(i, left, right);
            }

            this->left = left;
            this->right = right;
            
            finalize();
        }

        void update(INT i, INT* left, INT* right) {
            if (this->getTripleComponent(this->items[i]) != this->getTripleComponent(this->items[i - 1])) { // lef - left boundary of such an interval, rig - right boundary
                right[this->getTripleComponent(this->items[i - 1])] = i - 1;
                left[this->getTripleComponent(this->items[i])] = i;
            }
        }

        void finalize() {
            if (this->length > 0) {
                left[this->getTripleComponent(this->items[0])] = 0;
            }
            if (this->length > 1) {
                right[this->getTripleComponent(this->items[this->length - 1])] = this->length - 1;
            }
        }
    };

}

#endif
