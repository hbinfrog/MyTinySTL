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




}

#endif //MYTINYSTL_ALGOBASE_H
