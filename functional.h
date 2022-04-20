//
// Created by hbin on 2022/4/19.
//

#ifndef MYTINYSTL_FUNCTIONAL_H
#define MYTINYSTL_FUNCTIONAL_H
#include <cstddef>

namespace mystl{
    template <class Arg, class Result>
    struct unarg_function
    {
        typedef Arg       argument_type;
        typedef Result    result_type;
    };
    template <class Arg1, class Arg2, class Result>
    struct binary_function
    {
        typedef Arg1      first_argument_type;
        typedef Arg2      second_argument_type;
        typedef Result    result_type;
    };
    template <class T>
    struct equal_to :public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const { return x == y; }
    };
    template <class T>
    struct less :public binary_function<T, T, bool>
    {
        bool operator()(const T& x, const T& y) const { return x < y; }
    };

}

#endif //MYTINYSTL_FUNCTIONAL_H
