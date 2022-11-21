#ifndef TRIPLE_LIST_TRIPLE_LIST_H
#define TRIPLE_LIST_TRIPLE_LIST_H

#include "../main.h"

struct TripleList {
    Triple* items;

    INT length;

    INT* left;
    INT* right;

    INT (*getTripleComponent)(Triple triple);
    TripleElement element;

    TripleList(INT length, TripleElement element) {
        this->items = (Triple *)calloc(length, sizeof(Triple));
        this->length = length;
        this->element = element;

        switch (element) {
            case head:
                this->getTripleComponent = [](Triple triple){return triple.h;};
                break;
            case rel:
                this->getTripleComponent = [](Triple triple){return triple.r;};
                break;
            case tail:
                this->getTripleComponent = [](Triple triple){return triple.t;};
        }
    }

    void sort(INT nItems) { // number of unique relations or entities depending on this->element
        std::sort(
            this->items, this->items + this->length,
            // this->element == rel ? Triple::cmp_rel2 : this->element == ::TripleElement::head ? Triple::cmp_head : Triple::cmp_tail
            this->element == rel ? Triple::cmp_rel : this->element == ::TripleElement::head ? Triple::cmp_head : Triple::cmp_tail
        );

        INT* left = (INT *)calloc(nItems, sizeof(INT));
        INT* right = (INT *)calloc(nItems, sizeof(INT));

        memset(left, -1, sizeof(INT) * nItems);
        memset(right, -1, sizeof(INT) * nItems);

        for (INT i = 1; i < this->length; i++) { // get intervals for unique relationships in the test subset
            this->update(i, left, right);
        }

        this->left = left;
        this->right = right;
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

#endif
