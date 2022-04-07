//
// Created by hbin on 2022/3/18.
//

#ifndef MYTINYSTL_VECTOR_H
#define MYTINYSTL_VECTOR_H
#include <initializer_list>
#include "iterator.h"
#include "util.h"
#include "allocator.h"
#include "memory.h"



namespace mystl{
    template <class T>
    class vector{
    public:
        typedef mystl::allocator<T> allocator_type;
        typedef mystl::allocator<T> data_allocator;
        typedef typename allocator<T>::value_type value_type;
        typedef typename allocator<T>::pointer pointer;
        typedef typename allocator<T>::const_pointer const_pointer;
        typedef typename allocator<T>::reference reference;
        typedef typename allocator<T>::const_reference const_reference;
        typedef typename allocator<T>::size_type size_type;
        typedef typename allocator<T>::diff_type diff_type;

        typedef value_type* iterator;
        typedef const value_type* const_iterator;
        //typedef mystl::


    private:
        iterator begin;
        iterator end;
        iterator cap;
    public:
        vector() noexcept{
            init();
        }
        explicit vector(size_type n){
            fill_init(n, value_type());
        }
        size_type size(){
            return static_cast<size_type>(end - begin);
        }
        size_type capasity(){
            return static_cast<size_type>(cap - begin);
        }

    private:
        void init() noexcept;
        void init_space(size_type size, size_type cap);
        void fill_init(size_type n, const value_type& value);
    };


    template <class T>
    void vector<T>::init() noexcept {
        try{
            begin = data_allocator::allocate(16);
            end = begin;
            cap = begin + 16;
        }
        catch (...){
            begin = nullptr;
            end = nullptr;
            cap = nullptr;
        }

    }
    template <class T>
    void vector<T>::init_space(size_type size, size_type capa){
        try{
            begin = data_allocator::allocate(capa);
            end = begin + size;
            cap = begin + capa;
        }
        catch (...){
            begin = nullptr;
            end = nullptr;
            cap = nullptr;
            throw;
        }
    }
    template <class T>
    void vector<T>::fill_init(size_type n, const value_type &value){
        const size_type init_size = mystl::max(static_cast<size_type>(16), n);
        init_space(n, init_size);



    }

}

#endif //MYTINYSTL_VECTOR_H
