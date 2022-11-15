#ifndef TRIPLE_H
#define TRIPLE_H
#include "../Setting.h"
// #include "TripleIndex.h"

#include <functional>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>

enum TripleElement { head, rel, tail };
enum TripleComponent { entity, relation };

std::string getPluralTripleComponentName(TripleComponent component);
INT external_to_internal_id(INT external_id, INT* internal_id, std::unordered_map<INT, INT>* external_to_internal, std::vector<INT>* internal_to_external);

struct Triple {

	INT h, r, t;

    Triple(INT h_ = 0, INT r_ = 0, INT t_ = 0) {
       h = h_;
       r = r_;
       t = t_;
    }

	static INT minimal(INT a,INT b) {
		if (a > b) return b;
		return a;
	}
	
	static bool cmp_list(const Triple &a, const Triple &b) {
		return (minimal(a.h, a.t) > minimal(b.h, b.t));
	}

	static bool cmp_head(const Triple &a, const Triple &b) {
		return (a.h < b.h)||(a.h == b.h && a.r < b.r)||(a.h == b.h && a.r == b.r && a.t < b.t);
	}

	static bool cmp_tail(const Triple &a, const Triple &b) {
		return (a.t < b.t)||(a.t == b.t && a.r < b.r)||(a.t == b.t && a.r == b.r && a.h < b.h);
	}

	static bool cmp_rel(const Triple &a, const Triple &b) {
		return (a.h < b.h)||(a.h == b.h && a.t < b.t)||(a.h == b.h && a.t == b.t && a.r < b.r);
	}

	static bool cmp_rel2(const Triple &a, const Triple &b) {
		return (a.r < b.r)||(a.r == b.r && a.h < b.h)||(a.r == b.r && a.h == b.h && a.t < b.t);
	}

    void print() {
        std::cout << "head = " << h << ", rel = " << r << ", tail = " << t << "\n";
    }

    std::string as_filterable_string() {
        std::stringstream message;
        message << h << "\t" << r << "\t" << t;
        return message.str();
    }
};

struct TripleLists {
    Triple* main;
    Triple* head;
    Triple* tail;
    Triple* relation;
    INT length;

    TripleLists(INT length) {
        this->main = (Triple *)calloc(trainTotal, sizeof(Triple));
        this->head = (Triple *)calloc(trainTotal, sizeof(Triple));
        this->tail = (Triple *)calloc(trainTotal, sizeof(Triple));
        this->relation = (Triple *)calloc(trainTotal, sizeof(Triple));
        this->length = length;
    }

    TripleLists(Triple* main, INT length) {
        this->main = main;
        this->head = (Triple *)calloc(trainTotal, sizeof(Triple));
        this->tail = (Triple *)calloc(trainTotal, sizeof(Triple));
        this->relation = (Triple *)calloc(trainTotal, sizeof(Triple));
        this->length = length;
    }
};

struct Frequencies {
    INT* entity;
    INT* relation;

    INT nEntities;
    INT nRelations;

    Frequencies(INT nEntities, INT nRelations) {
        this->entity = (INT *)calloc(nEntities, sizeof(INT));
        this->relation = (INT *)calloc(nRelations, sizeof(INT));

        this->nEntities = nEntities;
        this->nRelations = nRelations;
    }
};

struct Boundaries {
    INT* left;
    INT* right;
    INT (*getTripleComponent)(Triple triple);

    INT length;

    Boundaries(INT length, INT (*getTripleComponent)(Triple triple)) {
        this->left = (INT *)calloc(length, sizeof(INT));
        this->right = (INT *)calloc(length, sizeof(INT));

        this->length = length;
        this->getTripleComponent = getTripleComponent;

        memset(this->right, -1, sizeof(INT) * length);
    }

    void update(Triple* triples, INT i) {
		if (this->getTripleComponent(triples[i]) != this->getTripleComponent(triples[i - 1])) { // lef - left boundary of such an interval, rig - right boundary
			this->right[this->getTripleComponent(triples[i - 1])] = i - 1;
			this->left[this->getTripleComponent(triples[i])] = i;
		}
    }

    void finalize(Triple* triples, INT nTriples) {
        this->left[this->getTripleComponent(triples[0])] = 0;
        this->right[this->getTripleComponent(triples[nTriples - 1])] = nTriples - 1;
    }
};

struct BoundaryCollection {
    Boundaries* head;
    Boundaries* tail;
    Boundaries* relation;

    BoundaryCollection(INT nEntities, INT nRelations) {
        this->head = new Boundaries(nEntities, [](Triple triple){return triple.h;});
        this->tail = new Boundaries(nEntities, [](Triple triple){return triple.t;});
        this->relation = new Boundaries(nRelations, [](Triple triple){return triple.r;});
    }

    BoundaryCollection(Frequencies* frequencies): BoundaryCollection(frequencies->nEntities, frequencies->nRelations) {}
};

Frequencies* dropDuplicates(Triple* main, Triple* heads, Triple* tails, Triple* relations, INT length, INT nEntities, INT nRelations);
BoundaryCollection* findBoundaries(TripleLists* lists, Frequencies* frequencies);

extern std::vector<INT> internal_to_external_entity_id;
extern std::vector<INT> internal_to_external_relation_id;

extern std::unordered_map<INT, INT> external_to_internal_entity_id;
extern std::unordered_map<INT, INT> external_to_internal_relation_id;

#endif
