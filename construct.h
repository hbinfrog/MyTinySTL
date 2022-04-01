//
// 对象的构造和析构
//

#ifndef MYTINYSTL_CONSTRUCT_H
#define MYTINYSTL_CONSTRUCT_H
#include <new>
#include "iterator.h"


namespace mystl{
    template <typename T>
    void construct(T * ptr){
        ::new (ptr) T();/*在堆区创建了一个新的数据，默认值，跟int * a = new int();的作用差不多
                        但是这里用int * a = new int();不能够起作用*/
    }
    template <typename T1, typename T2>
    void construct(T1 * ptr, const T2& value){
        ::new (ptr) T1(value);//new前面加不加::
    }
    template <typename T1, typename... Args>
    void construct(T1* ptr, Args&&...args){
        //
    }
    template <typename T>
    void destroy_one(T * pointer, std::true_type){}
    template <typename T>
    void destroy_one(T * pointer, std::false_type){
        if(pointer != nullptr){
            pointer->~T();
        }
    }
    template <typename T>
    void destroy_sec(T first, T last, std::true_type){}
    template <typename T>
    void destroy_sec(T first, T last, std::false_type){
        while(first != last){
            destroy(&*first);
            first++;
        }
    }

    template <typename T>
    void destroy(T * ptr){
        destroy_one(ptr, std::is_trivially_destructible<T>{});

    }
    template <typename T>
    void destroy(T first, T last){
        destroy_sec(first, last, std::is_trivially_destructible<typename std::iterator_traits<T>::value_type>{});

    }//





}

#endif //MYTINYSTL_CONSTRUCT_H
