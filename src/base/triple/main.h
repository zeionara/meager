#ifndef TRIPLE_H
#define TRIPLE_H

#include "../types.h"

#include <cstring>
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

TripleElement decodeTripleElement(std::string name);

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

Frequencies* dropDuplicates(Triple* main, Triple* heads, Triple* tails, Triple* relations, INT length, INT nEntities, INT nRelations);

#endif
