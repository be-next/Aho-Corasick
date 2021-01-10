//
//  find.hpp
//  Aho_Corasick
//
//  Created by Jérôme Ramette on 09/01/2021.
//

#ifndef find_hpp
#define find_hpp

#include <stdio.h>

namespace bn_std {

template<class InputIterator, class T>
InputIterator find (InputIterator first, InputIterator last, const T& val)
{
    while (first!=last) {
        if (*first==val) return first;
        ++first;
    }
    return last;
}

} // end bn_std

#endif /* find_hpp */
