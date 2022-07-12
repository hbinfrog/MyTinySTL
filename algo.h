//
// Created by hbin on 2022/4/26.
//

#ifndef MYTINYSTL_ALGO_H
#define MYTINYSTL_ALGO_H
#include <cstddef>
#include <ctime>

#include "algobase.h"
#include "memory.h"
//#include "heap_algo.h"
#include "functional.h"

namespace mystl{
    template <class BidirectionalIter>
    void reverse_dispatch(BidirectionalIter first, BidirectionalIter last,
                          bidirectional_iterator_tag)
    {
        while (true)
        {
            if (first == last || first == --last)
                return;
            mystl::iter_swap(first++, last);
        }
    }

// reverse_dispatch 的 random_access_iterator_tag 版本
    template <class RandomIter>
    void reverse_dispatch(RandomIter first, RandomIter last,
                          random_access_iterator_tag)
    {
        while (first < last)
            mystl::iter_swap(first++, --last);
    }

    template <class BidirectionalIter>
    void reverse(BidirectionalIter first, BidirectionalIter last)
    {
        mystl::reverse_dispatch(first, last, iterator_category(first));
    }

    template <class ForwardIter, class T>
    ForwardIter
    lbound_dispatch(ForwardIter first, ForwardIter last,
                    const T& value, forward_iterator_tag)
    {
        auto len = mystl::distance(first, last);
        auto half = len;
        ForwardIter middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first;
            mystl::advance(middle, half);
            if (*middle < value)
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else
            {
                len = half;
            }
        }
        return first;
    }

// lbound_dispatch 的 random_access_iterator_tag 版本
    template <class RandomIter, class T>
    RandomIter
    lbound_dispatch(RandomIter first, RandomIter last,
                    const T& value, random_access_iterator_tag)
    {
        auto len = last - first;
        auto half = len;
        RandomIter middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first + half;
            if (*middle < value)
            {
                first = middle + 1;
                len = len - half - 1;
            }
            else
            {
                len = half;
            }
        }
        return first;
    }

    template <class ForwardIter, class T>
    ForwardIter
    lower_bound(ForwardIter first, ForwardIter last, const T& value)
    {
        return mystl::lbound_dispatch(first, last, value, iterator_category(first));
    }
    template <class ForwardIter, class T, class Compared>
    ForwardIter
    lbound_dispatch(ForwardIter first, ForwardIter last,
                    const T& value, forward_iterator_tag, Compared comp)
    {
        auto len = mystl::distance(first, last);
        auto half = len;
        ForwardIter middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first;
            mystl::advance(middle, half);
            if (comp(*middle, value))
            {
                first = middle;
                ++first;
                len = len - half - 1;
            }
            else
            {
                len = half;
            }
        }
        return first;
    }
    template <class RandomIter, class T, class Compared>
    RandomIter
    lbound_dispatch(RandomIter first, RandomIter last,
                    const T& value, random_access_iterator_tag, Compared comp)
    {
        auto len = last - first;
        auto half = len;
        RandomIter middle;
        while (len > 0)
        {
            half = len >> 1;
            middle = first + half;
            if (comp(*middle, value))
            {
                first = middle + 1;
                len = len - half - 1;
            }
            else
            {
                len = half;
            }
        }
        return first;
    }

    template <class ForwardIter, class T, class Compared>
    ForwardIter
    lower_bound(ForwardIter first, ForwardIter last, const T& value, Compared comp)
    {
        return mystl::lbound_dispatch(first, last, value, iterator_category(first), comp);
    }

    template <class ForwardIter1, class ForwardIter2, class BinaryPred>
    bool is_permutation_aux(ForwardIter1 first1, ForwardIter1 last1,
                            ForwardIter2 first2, ForwardIter2 last2,
                            BinaryPred pred)
    {
        constexpr bool is_ra_it = mystl::is_random_access_iterator<ForwardIter1>::value
                                  && mystl::is_random_access_iterator<ForwardIter2>::value;
        if (is_ra_it)
        {
            auto len1 = last1 - first1;
            auto len2 = last2 - first2;
            if (len1 != len2)
                return false;
        }

        // 先找出相同的前缀段
        for (; first1 != last1 && first2 != last2; ++first1, (void) ++first2)
        {
            if (!pred(*first1, *first2))
                break;
        }
        if (is_ra_it)
        {
            if (first1 == last1)
                return true;
        }
        else
        {
            auto len1 = mystl::distance(first1, last1);
            auto len2 = mystl::distance(first2, last2);
            if (len1 == 0 && len2 == 0)
                return true;
            if (len1 != len2)
                return false;
        }

        // 判断剩余部分
        for (auto i = first1; i != last1; ++i)
        {
            bool is_repeated = false;
            for (auto j = first1; j != i; ++j)
            {
                if (pred(*j, *i))
                {
                    is_repeated = true;
                    break;
                }
            }

            if (!is_repeated)
            {
                // 计算 *i 在 [first2, last2) 的数目
                auto c2 = 0;
                for (auto j = first2; j != last2; ++j)
                {
                    if (pred(*i, *j))
                        ++c2;
                }
                if (c2 == 0)
                    return false;

                // 计算 *i 在 [first1, last1) 的数目
                auto c1 = 1;
                auto j = i;
                for (++j; j != last1; ++j)
                {
                    if (pred(*i, *j))
                        ++c1;
                }
                if (c1 != c2)
                    return false;
            }
        }
        return true;
    }

    template <class ForwardIter1, class ForwardIter2, class BinaryPred>
    bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
                        ForwardIter2 first2, ForwardIter2 last2,
                        BinaryPred pred)
    {
        return is_permutation_aux(first1, last1, first2, last2, pred);
    }

    template <class ForwardIter1, class ForwardIter2>
    bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
                        ForwardIter2 first2, ForwardIter2 last2)
    {
        typedef typename iterator_traits<ForwardIter1>::value_type v1;
        typedef typename iterator_traits<ForwardIter2>::value_type v2;
        static_assert(std::is_same<v1, v2>::value,
                      "the type should be same in mystl::is_permutation");
        return is_permutation_aux(first1, last1, first2, last2,
                                  mystl::equal_to<v1>());
    }
}


#endif //MYTINYSTL_ALGO_H
