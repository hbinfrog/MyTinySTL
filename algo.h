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
}


#endif //MYTINYSTL_ALGO_H
