//
// Created by hbin on 2022/3/18.
//

#ifndef MYTINYSTL_ITERATOR_H
#define MYTINYSTL_ITERATOR_H
#include "type_traits.h"
#include <cstddef>
namespace mystl{
    template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T *, class Reference = T &>
    struct iterator{
        typedef Category iterator_category;
        typedef T value_type;
        typedef Pointer pointer;
        typedef Reference reference;
        typedef Distance difference_type;
    };


}

#endif //MYTINYSTL_ITERATOR_H
