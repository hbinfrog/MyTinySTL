//
// Created by hbin on 2022/3/18.
//

#ifndef MYTINYSTL_ALLOCATOR_H
#define MYTINYSTL_ALLOCATOR_H
#include <cstddef>
#include "construct.h"

namespace mystl{
    template <class T>
    class allocator{
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t diff_type;

        static pointer allocate();
        static pointer allocate(size_type n);
        static void deallocate(pointer ptr);
        static void deallocate(pointer ptr, size_type n);
        static void construct(pointer ptr);
        static void construct(pointer ptr, const_reference value);
        static void construct(pointer ptr, T&& value);

        template<class... Args>
        static void construct(pointer ptr, Args&& ...args);
        static void destroy(pointer ptr);
        static void destroy(pointer first, pointer last);


    };
    template <class T>
    T* allocator<T>::allocate(){
        return static_cast<T*> (operator new(sizeof(T)));

    }
    template <class T>
    T* allocator<T>::allocate(size_type n){
        if(n == 0)
            return nullptr;
        return static_cast<T*> (operator new(n * sizeof(T)));

    }
    template <class T>
    void allocator<T>::deallocate(T * ptr){
        if(ptr == nullptr)
            return;
        ::operator delete(ptr);

    }
    template <class T>
    void allocator<T>::deallocate(T * ptr, size_type n){
        if(ptr == nullptr)
            return;
        ::operator delete(ptr);



    }

    template <class T>
    void allocator<T>::construct(T* ptr){
        mystl::construct(ptr);
    }
    template <class T>
    void allocator<T>::construct(pointer ptr, const_reference value){
        mystl::construct(ptr, value);
    }
    template <class T>
    void allocator<T>::construct(T * ptr, T&& value){
        mystl::construct(ptr, mystl::move(value));
    }
    template <class T>
    template <class ...Args>
    void allocator<T>::construct(T* ptr, Args&& ...args)
    {
        //mystl::construct(ptr, mystl::forward<Args>(args)...);
        mystl::construct(ptr, mystl::forward<Args>(args)...);
    }
    template <class T>
    void allocator<T>::destroy(T* ptr){
        mystl::destroy(ptr);
    }

    template <class T>
    void allocator<T>::destroy(pointer first, pointer last){
        mystl::destroy(first, last);
    }



}



#endif //MYTINYSTL_ALLOCATOR_H
