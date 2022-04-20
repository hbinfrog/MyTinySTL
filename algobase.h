//
// Created by hbin on 2022/4/7.
//

#ifndef MYTINYSTL_ALGOBASE_H
#define MYTINYSTL_ALGOBASE_H
#include <cstring>
#include "iterator.h"
#include "util.h"

namespace mystl{
    template <class T>
    const T& max(const T& a, const T& b){
        return a < b ? b : a;
    }
    template <class T, class Compare>
    const T& max(const T& a, const T& b, Compare compare){
        return compare(a, b) ? b : a;
    }
    template <class T>
    const T& min(const T& a, const T& b){
        return a < b ? a : b;
    }
    template <class T, class Compare>
    const T& min(const T& a, const T& b, Compare compare){
        return compare(a, b) ? a : b;
    }
    template <class Fiter1, class Fiter2>
    void iter_swap(Fiter1 a, Fiter2 b){
        mystl::swap(*a,*b);
    }
    template <class InputIter, class OutputIter>
    OutputIter unchecked_copy_sec(InputIter first, InputIter last, OutputIter result, mystl::input_iterator_tag){
        for(; first != last; first++, result++){
            *result = *first;
        }
        return result;
    }
    template <class RandomIter, class OutputIter>
    OutputIter unchecked_copy_sec(RandomIter first, RandomIter last, OutputIter result, mystl::random_access_iterator_tag){
        for(auto n = last - first; n > 0; n--, first++, result++){
            *result = *first;
        }
        return result;
    }
    template <class InputIter, class OutputIter>
    OutputIter unchecked_copy(InputIter first, InputIter last, OutputIter result){
        return unchecked_copy_sec(first,last,result, iterator_category(first));
    }
    template <class Tp, class Up>
    typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type, Up>::value
                            && std::is_trivially_copy_assignable<Up>::value, Up*>::type
    unchecked_copy(Tp* first, Tp* last, Up* result){
        const auto n = static_cast<size_t>(last - first);
        if(n != 0){
            std::memmove(result, first, n * sizeof(Up));
        }
        return result + n;

    }
    template <class InputIter, class OutputIter>
    OutputIter copy(InputIter first, InputIter last, OutputIter result){
        return unchecked_copy(first, last, result);
    }
    template <class B1, class B2>
    B2 unchecked_move_back_sec(B1 first, B1 last, B2 result, mystl::bidirectional_iterator_tag){
        while(first != last){
            result--;
            last--;
            *result = mystl::move(*last);
        }
        return result;
    }
    template <class B1, class B2>
    B2 unchecked_move_back_sec(B1 first, B1 last, B2 result, mystl::random_access_iterator_tag){
        for(auto n = last - first; n > 0; n--){
            result--;
            last--;
            *result = mystl::move(*last);
        }
        return result;
    }
    template <class B1, class B2>
    B2 unchecked_move_back(B1 first, B1 last, B2 result){
        return unchecked_move_back_sec(first, last, result, iterator_category(first));
    }
    template <class Tp, class Up>
    typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type, Up>::value
    && std::is_trivially_copy_assignable<Up>::value, Up*>::type
    unchecked_copy_back(Tp* first, Tp* last, Up* result){
        const auto n = static_cast<size_t>(last - first);
        if(n != 0){
            result = result - n;
            std::memmove(result, first, sizeof(Up) * n);
        }
        return result;
    }
    template <class B1, class B2>
    B2 copy_back(B1 first, B1 last, B2 result){
        return unchecked_copy_back(first, last, result);
    }
    /*
     copy_if 将[first, last)内满足一元操作 unary_pred 的元素拷贝到以 result 为起始的位置上
     */
    template <class InputIter, class OutputIter, class UnaryPredicate>
    OutputIter copy_if(InputIter first, InputIter last, OutputIter result, UnaryPredicate unary_pred){
        for(; first != last; ++first){
            if(unary_pred(*first)){
                *result = *first;
                result++;
            }
        }
        return result;
    }
    /*
     copy_n 将[first, first + n)内的元素拷贝到以 [result, result + n) 上
     */
    template <class InputIter, class Size, class OutputIter>
    mystl::pair<InputIter, OutputIter> unchecked_copy_n(InputIter first, Size n, OutputIter result, mystl::input_iterator_tag){
        while(n > 0){
            *result = *first;
            n--;
            result++;
            first++;
        }
        return mystl::pair<InputIter,OutputIter>(first, result);
    }
    template <class RandomIter, class Size, class OutputIter>
    mystl::pair<RandomIter, OutputIter> unchecked_copy_n(RandomIter first, Size n, OutputIter result, mystl::random_access_iterator_tag){
        auto last = first + n;
        return mystl::pair<RandomIter, OutputIter>(last, mystl::copy(first,last,result));
    }
    template <class InputIter, class Size, class OutputIter>
    mystl::pair<InputIter,OutputIter> copy_n(InputIter first, Size n, OutputIter result){
        return unchecked_copy_n(first,n,result, iterator_category(first));
    }
    /*
     move, 把 [first, last)区间内的元素移动到 [result, result + (last - first))内
     */
    template <class InputIter, class OutputIter>
    OutputIter unchecked_move_sec(InputIter first, InputIter last, OutputIter result, mystl::input_iterator_tag){
        while(first != last){
            *result = mystl::move(*first);
            first++;
            result++;
        }
        return result;
    }
    template <class RandomIter, class OutputIter>
    OutputIter unchecked_move_sec(RandomIter first, RandomIter last, OutputIter result, mystl::random_access_iterator_tag){
        auto n = last - first;
        while(n > 0){
            *result = mystl::move(*first);
            n--;
            result++;
            first++;
        }
        return result;
    }
    template <class InputIter, class OutputIter>
    OutputIter unchecked_move(InputIter first, InputIter last, OutputIter result){
        return unchecked_move_sec(first, last, result, iterator_category(first));
    }
    template <class Tp, class Up>
    typename std::enable_if<
            std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
            std::is_trivially_move_assignable<Up>::value,
            Up*>::type
    unchecked_move(Tp* first, Tp* last, Up* result)
    {
        const size_t n = static_cast<size_t>(last - first);
        if (n != 0)
            std::memmove(result, first, n * sizeof(Up));
        return result + n;
    }

    template <class InputIter, class OutputIter>
    OutputIter move(InputIter first, InputIter last, OutputIter result)
    {
        return unchecked_move(first, last, result);
    }






    template <class OutputIter, class Size, class T>
    OutputIter unchecked_fill_n(OutputIter first, Size n, const T& val){
        while(n > 0){
            *first = val;
            --n;
            ++first;
        }
        return first;
    }
    template <class Tp, class Size, class Up>
    typename std::enable_if<
            std::is_integral<Tp>::value && sizeof(Tp) == 1 &&
            !std::is_same<Tp, bool>::value &&
            std::is_integral<Up>::value && sizeof(Up) == 1,
            Tp*>::type
    unchecked_fill_n(Tp* first, Size n, Up value)
    {
        if (n > 0)
        {
            std::memset(first, (unsigned char)value, (size_t)(n));
        }
        return first + n;
    }
    template <class OutputIter, class Size, class T>
    OutputIter fill_n(OutputIter first, Size n, const T& val){
        return unchecked_fill_n(first,n,val);
    }

    template <class InputIter, class OutputIter>
    bool equal(InputIter first1, InputIter last1, OutputIter first2){
        while (first1 != last1){
            if(*first1 != *last1){
                return false;
            }
            first1++;
            first2++;
        }
        return true;
    }
    template <class InputIter, class OutputIter, class Compare>
    bool equal(InputIter first1, InputIter last1, OutputIter first2, Compare comp){
        while (first1 != last1){
            if(!(comp(first1,first2))){
                return false;
            }
            first1++;
            first2++;
        }
        return true;
    }

    template <class InputIter1, class InputIter2>
    bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                                 InputIter2 first2, InputIter2 last2)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if (*first1 < *first2)
                return true;
            if (*first2 < *first1)
                return false;
        }
        return first1 == last1 && first2 != last2;
    }
    template <class InputIter1, class InputIter2, class Compred>
    bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                                 InputIter2 first2, InputIter2 last2, Compred comp)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
        {
            if (comp(*first1, *first2))
                return true;
            if (comp(*first2, *first1))
                return false;
        }
        return first1 == last1 && first2 != last2;
    }

    bool lexicographical_compare(const unsigned char* first1, const unsigned char* last1, const unsigned char* first2, const unsigned char* last2){
        const auto len1 = last1 - first1;
        const auto len2 = last2 - first2;
        const auto result = std::memcmp(first1, first2, mystl::min(len1, len2));
        return result != 0 ? result < 0 : len1 < len2;
    }




}

#endif //MYTINYSTL_ALGOBASE_H
