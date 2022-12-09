#ifndef TRIPLE_TRIPLE_H
#define TRIPLE_TRIPLE_H

#include <iostream>
#include <string>
#include <sstream>

#include "../types.h"

using namespace std;

namespace meager::main::triple {

    struct Triple {

        INT h, r, t;

        Triple(INT h_ = 0, INT r_ = 0, INT t_ = 0) {
           h = h_;
           r = r_;
           t = t_;
        }

        // static INT minimal(INT a,INT b) {
        //     if (a > b) return b;
        //     return a;
        // }
        // 
        // static bool cmp_list(const Triple &a, const Triple &b) {
        //     return (minimal(a.h, a.t) > minimal(b.h, b.t));
        // }

        static bool compareHeadRelationTail(const Triple& a, const Triple& b) {
            return (a.h < b.h) || (a.h == b.h && a.r < b.r) || (a.h == b.h && a.r == b.r && a.t < b.t);
        }

        static bool compareTailRelationHead(const Triple &a, const Triple &b) {
            return (a.t < b.t) || (a.t == b.t && a.r < b.r) || (a.t == b.t && a.r == b.r && a.h < b.h);
        }

        static bool compareHeadTailRelation(const Triple &a, const Triple &b) {
            return (a.h < b.h) || (a.h == b.h && a.t < b.t) || (a.h == b.h && a.t == b.t && a.r < b.r);
        }

        static bool compareRelationHeadTail(const Triple &a, const Triple &b) {
            return (a.r < b.r)||(a.r == b.r && a.h < b.h)||(a.r == b.r && a.h == b.h && a.t < b.t);
        }

        void print() {
            cout << h << ":" << r << ":" << t << "\n";
        }

        string asFilterableString() {
            stringstream message;
            message << h << "\t" << r << "\t" << t;
            return message.str();
        }
    };

}

#endif
