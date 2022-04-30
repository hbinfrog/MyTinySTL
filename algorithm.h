//
// Created by hbin on 2022/4/30.
//

#ifndef MYTINYSTL_ALGORITHM_H
#define MYTINYSTL_ALGORITHM_H
#include "iterator.h"
#include "util.h"

namespace mystl{

    template <class RandomIter>
    void heap_down(RandomIter first, RandomIter last, RandomIter target){
        auto t = target - first;
        auto size_ = last - first;
        auto left = 2 * (target - first) + 1, right = 2 * (target - first) + 2;
        if(left < size_ && *(first + left)  > *(t + first)) t = left;
        if(right < size_ && *(first + right)  > *(t + first)) t = right;
        if(*(t + first) != *target){
            mystl::swap(*(t+first), *target);
            heap_down(first,last, t + first);
        }
    }
    template <class RandomIter>
    void heap_up(RandomIter first, RandomIter last, RandomIter target){
        auto parent = (target - first - 1) / 2;
        if(parent >= 0 && *(first + parent)  < *target){
            mystl::swap(*(first + parent), *(target));
            heap_up(first,last,first + parent);
        }
    }

    template <class RandomIter>
    void push_heap(RandomIter first, RandomIter last){
        heap_up(first, last, last - 1);
    }

    template <class RandomIter, class Compare>
    void heap_up(RandomIter first, RandomIter last, RandomIter target, Compare comp){
        auto parent = (target - first - 1) / 2;
        if(parent >= 0 && comp(*(first + parent), *target)){
            mystl::swap(*(first + parent), *(target));
            heap_up(first,last,first + parent,comp);
        }
    }

    template <class RandomIter, class Compared>
    void push_heap(RandomIter first, RandomIter last, Compared comp){
        heap_up(first, last, last - 1, comp);
    }
    template <class RandomIter>
    void pop_heap(RandomIter first, RandomIter last){
        mystl::swap(*first, *(last - 1));
        heap_down(first,last - 1,first);
    }

    template <class RandomIter, class Compare>
    void heap_down(RandomIter first, RandomIter last, RandomIter target, Compare comp){
        auto t = target - first;
        auto size_ = last - first;
        auto left = 2 * (target - first) + 1, right = 2 * (target - first) + 2;
        if(left < size_ && comp(*(t + first), *(first + left))) t = left;
        if(right < size_ && comp(*(t + first), *(first + right))) t = right;
        if(*(t + first) != *target){
            mystl::swap(*(t+first), *target);
            heap_down(first,last, t + first);
        }
    }
    template <class RandomIter, class Compared>
    void pop_heap(RandomIter first, RandomIter last, Compared comp){
        mystl::swap(*first, *(last - 1));
        heap_down(first,last - 1,first,comp);
    }

    template <class RandomIter>
    void make_heap(RandomIter first, RandomIter last){
        for(auto i = (last - first - 1) / 2; i >= 0; i--){
            heap_down(first, last, first + i);
        }
    }

    template <class RandomIter, class Compared>
    void make_heap(RandomIter first, RandomIter last, Compared comp){
        for(auto i = (last - first - 1) / 2; i >= 0; i--){
            heap_down(first, last, first + i, comp);
        }

    }

}

#endif //MYTINYSTL_ALGORITHM_H
