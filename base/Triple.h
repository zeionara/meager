#ifndef TRIPLE_H
#define TRIPLE_H
#include "Setting.h"
#include <iostream>


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
};

#endif
