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
    template<bool b>
    using my_bool_constant = my_integral_constant<bool, b>;
    typedef my_bool_constant<true> my_true_type;
    typedef my_bool_constant<false> my_false_type;
    template <class T1, class T2>
    struct pair;
    template <class T>
    struct is_pair : mystl::my_false_type {};

    template <class T1, class T2>
    struct is_pair<mystl::pair<T1, T2>> : mystl::my_true_type {};

}

#endif //MYTINYSTL_TYPE_TRAITS_H
