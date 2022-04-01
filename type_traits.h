//
// Created by hbin on 2022/4/1.
//

#ifndef MYTINYSTL_TYPE_TRAITS_H
#define MYTINYSTL_TYPE_TRAITS_H
#include <type_traits>

namespace mystl{
    template<class T, T v>
    struct my_integral_constant{
        static constexpr T value = v;
    };
    typedef my_integral_constant<bool, true> my_true_type;
    typedef my_integral_constant<bool, false> my_false_type;



}

#endif //MYTINYSTL_TYPE_TRAITS_H
