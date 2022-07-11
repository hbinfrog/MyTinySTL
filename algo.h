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
}


#endif //MYTINYSTL_ALGO_H
