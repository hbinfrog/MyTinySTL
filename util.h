//
// Created by hbin on 2022/4/1.
//

#ifndef MYTINYSTL_UTIL_H
#define MYTINYSTL_UTIL_H
#include "type_traits.h"
#include <cstddef>


namespace mystl{

    //move
    template <class T>
    typename std::remove_reference<T>::type&& move(T&& arg) noexcept{
        return static_cast<typename std::remove_reference<T>::type&&>(arg);
    }
    //forward
    template <class T>
    T&& forward(typename std::remove_reference<T>::type& arg) noexcept{
        return static_cast<T&&>(arg);
    }
    template <class T>
    T&& forward(typename std::remove_reference<T>::type&& arg) noexcept{
        static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
        return static_cast<T&&>(arg);
    }
    //swap
    template <class T>
    void swap(T& a, T& b){
        T temp = mystl::move(a);
        /*
         T&& res =  mystl::move(a);
         T temp = res;
        */
        a = mystl::move(b);
        b = mystl::move(temp);
    }

    template <class Tp1, class Tp2>
    Tp2 swap_range(Tp1 first1, Tp1 last1, Tp2 first2){
        for(; first1 != last1; first1++, first2++){
            mystl::swap(*first1, *first2);
        }
        return first2;
    }
    template <class Tp, size_t N>
    void swap(Tp(&a)[N], Tp(&b)[N]){
        mystl::swap_range(a,a+N,b);
    }
    template <class T1, class T2>
    struct pair{
        typedef T1 first_type;
        typedef T2 second_type;
        first_type first;
        second_type second;
        template<class U1 = T1, class U2 = T2, typename = typename std::enable_if<std::is_default_constructible<U1>::value
        && std::is_default_constructible<U2>::value, void>::type>
        constexpr pair():first(), second(){}
        constexpr pair(const T1& a, const T2& b):first(a), second(b){}
        template <class U1 = T1, class U2 = T2,
                typename std::enable_if<
                std::is_copy_constructible<U1>::value &&
                std::is_copy_constructible<U2>::value &&
                std::is_convertible<const U1&, T1>::value &&
                std::is_convertible<const U2&, T2>::value, int>::type = 0>
        constexpr pair(const T1& e, const T2& f): first(e), second(f){}
        // explicit constructible for this type
        template <class U1 = T1, class U2 = T2,
                typename std::enable_if<
                std::is_copy_constructible<U1>::value &&
                std::is_copy_constructible<U2>::value &&
                (!std::is_convertible<const U1&, T1>::value ||
                 !std::is_convertible<const U2&, T2>::value), int>::type = 0>
        explicit constexpr pair(const T1& g, const T2& h): first(g), second(h){}
        pair(const pair& p) = default;
        pair(pair&& p) = default;
        template <class U1, class U2,
                typename std::enable_if<
                std::is_copy_constructible<U1>::value &&
                std::is_copy_constructible<U2>::value &&
                std::is_convertible<const U1&&, T1>::value &&
                std::is_convertible<const U2&&, T2>::value, int>::type = 0>
        constexpr pair(U1&& c, U2&& d): first(mystl::forward<U1>(c)), second(mystl::forward<U2>(d)){}
        // explicit constructiable for other type
        template <class Other1, class Other2,
                typename std::enable_if<
                std::is_constructible<T1, Other1>::value &&
                std::is_constructible<T2, Other2>::value &&
                (!std::is_convertible<Other1, T1>::value ||
                 !std::is_convertible<Other2, T2>::value), int>::type = 0>
        explicit constexpr pair(Other1&& a, Other2&& b) : first(mystl::forward<Other1>(a)), second(mystl::forward<Other2>(b)){}
        template <class Other1, class Other2,
                typename std::enable_if<
                std::is_constructible<T1, const Other1&>::value &&
                std::is_constructible<T2, const Other2&>::value &&
                std::is_convertible<const Other1&, T1>::value &&
                std::is_convertible<const Other2&, T2>::value, int>::type = 0>
        constexpr pair(const pair<Other1, Other2>& other) : first(other.first), second(other.second){}
        // explicit constructiable for other pair
        template <class Other1, class Other2,
                typename std::enable_if<
                std::is_constructible<T1, const Other1&>::value &&
                std::is_constructible<T2, const Other2&>::value &&
                (!std::is_convertible<const Other1&, T1>::value ||
                 !std::is_convertible<const Other2&, T2>::value), int>::type = 0>
        explicit constexpr pair(const pair<Other1, Other2>& other) : first(other.first), second(other.second){}
        // implicit constructiable for other pair
        template <class Other1, class Other2,
                typename std::enable_if<
                std::is_constructible<T1, Other1>::value &&
                std::is_constructible<T2, Other2>::value &&
                std::is_convertible<Other1, T1>::value &&
                std::is_convertible<Other2, T2>::value, int>::type = 0>
        constexpr pair(pair<Other1, Other2>&& other) : first(mystl::forward<Other1>(other.first)), second(mystl::forward<Other2>(other.second)){}

        // explicit constructiable for other pair
        template <class Other1, class Other2,
                typename std::enable_if<
                std::is_constructible<T1, Other1>::value &&
                std::is_constructible<T2, Other2>::value &&
                (!std::is_convertible<Other1, T1>::value ||
                 !std::is_convertible<Other2, T2>::value), int>::type = 0>
        explicit constexpr pair(pair<Other1, Other2>&& other) : first(mystl::forward<Other1>(other.first)), second(mystl::forward<Other2>(other.second)){}

        pair& operator=(const pair& rhs){
            first = rhs.first;
            second = rhs.second;
            return  *this;
        }
        pair& operator=(pair&& rhs){
            first = mystl::move(rhs.first);
            second = mystl::move(rhs.second);
            return *this;
        }

        template<class U1, class U2>
        pair& operator=(const pair<U1, U2>& other){
            first = other.first;
            second = other.second;
            return *this;
        }
        template<class U1, class U2>
        pair& operator=(const pair<U1, U2>&& others){
            first = mystl::forward<U1>(others.first);
            second = mystl::forward<U2>(others.second);
            return *this;
        }
        ~pair() = default;
        void swap(pair& other){
            mystl::swap(first,other.first);
            mystl::swap(second, other.second);

        }
        bool operator==(const pair<T1, T2> p){
            return first == p.first && second == p.second;
        }
        bool operator<(const pair<T1, T2> p){
            return first < p.first || (first == p.first && second < p.second);
        }
        bool operator!=(const pair<T1, T2> p){
            return !(this->operator==(p));
        }
        bool operator>(const pair<T1, T2> p){
            return p.first < first || (p.first == first && p.second < second);
        }
        bool operator<=(const pair<T1, T2> p){
            return !(this->operator>(p));
        }
        bool operator>=(const pair<T1, T2> p){
            return !(this->operator<(p));
        }



    };
    template <class T1, class T2>
    void swap(pair<T1, T2>& p1, pair<T1, T2>& p2){
        p1.swap(p2);
    }
    template <class T1, class T2>
    pair<T1, T2> make_pair(T1&& first, T2&& second){
        return pair<T1, T2>(mystl::forward<T1>(first), mystl::forward<T2>(second));
    }


}

#endif //MYTINYSTL_UTIL_H
