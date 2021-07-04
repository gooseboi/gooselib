#pragma once

#include <cstdio>

namespace goose {
    template<class It1, class It2>
    bool lexicographicalCompare(It1 first1, It1 last1,
                                 It2 first2, It2 last2) {
        for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
            if (*first1 < *first2) return true;
            if (*first2 < *first1) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }
}