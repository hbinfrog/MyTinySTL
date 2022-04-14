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
#include "algobase.h"
#include "exceptdef.h"
#include "uninitialized.h"



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
        iterator begin_;
        iterator end_;
        iterator cap_;
    public:
        vector() noexcept{
            init();
        }
        explicit vector(size_type n){
            fill_init(n, value_type());
        }
        vector(size_type n, const value_type& val){
            fill_init(n,val);
        }
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        vector(Iter first, Iter last)
        {
            MYSTL_DEBUG(!(last < first));
            range_init(first, last);
        }
        vector(const vector& rhs){
            range_init(rhs.begin_, rhs.end_);
        }
        vector(vector&& rhs) noexcept : begin_(rhs.begin_), end_(rhs.end_), cap_(rhs.cap_){
            rhs.begin_ = nullptr;
            rhs.end_ = nullptr;
            rhs.end_ = nullptr;//the source of rvalue type should be nullptr
        }
        vector(std::initializer_list<value_type> initlist){
            range_init(initlist.begin(),initlist.end());
        }
        vector& operator=(vector& rhs);
        vector& operator=(vector&& rhs) noexcept;
        vector& operator=(std::initializer_list<value_type> initlist);
        void swap(vector& rhs) noexcept;
        bool operator==(vector& rhs);
        bool operator!=(vector& rhs);



        size_type size(){
            return static_cast<size_type>(end_ - begin_);
        }
        size_type capasity(){
            return static_cast<size_type>(cap_ - begin_);
        }
        reference operator[](size_type n){
            MYSTL_DEBUG(n < size());
            return *(begin_ + n);
        }
        const_reference operator[](size_type n) const{
            MYSTL_DEBUG(n < size());
            return *(begin_ + n);
        }
        iterator begin() noexcept{
            return begin_;
        }
        const_iterator begin() const noexcept{
            return begin_;
        }
        iterator end() noexcept{
            return end_;
        }
        const iterator end() const noexcept{
            return end_;
        }
        iterator cap() noexcept{
            return cap_;
        }
        const iterator cap() const noexcept{
            return cap_;
        }


    private:
        void init() noexcept;
        void init_space(size_type size, size_type cap_);
        void fill_init(size_type n, const value_type& value);

        template<class Iter>
        void range_init(Iter first, Iter last);
    };


    template <class T>
    void vector<T>::init() noexcept {
        try{
            begin_ = data_allocator::allocate(16);
            end_ = begin_;
            cap_ = begin_ + 16;
        }
        catch (...){
            begin_ = nullptr;
            end_ = nullptr;
            cap_ = nullptr;
        }

    }
    template <class T>
    void vector<T>::init_space(size_type size, size_type cap){
        try{
            begin_ = data_allocator::allocate(cap);
            end_ = begin_ + size;
            cap_ = begin_ + cap;
        }
        catch (...){
            begin_ = nullptr;
            end_ = nullptr;
            cap_ = nullptr;
            throw;
        }
    }
    template <class T>
    void vector<T>::fill_init(size_type n, const value_type &value){
        const size_type init_size = mystl::max(static_cast<size_type>(16), n);
        init_space(n, init_size);
        mystl::uninitialized_fill_n(begin_, n, value);
    }
    template <class T>
    template<class Iter>
    void vector<T>::range_init(Iter first, Iter last){
        const size_type init_size = mystl::max(static_cast<size_type>(last - first), static_cast<size_type>(16));
        init_space(last - first, init_size);
        mystl::copy(first,last,begin_);

    }
    template <class T>
    void vector<T>::swap(vector<T> &rhs) noexcept{
        if(this->operator!=(rhs)){
            mystl::swap(this->begin_, rhs.begin_);
            mystl::swap(this->end_, rhs.end_);
            mystl::swap(this->cap_, rhs.cap_);
        }

    }
    template <class T>
    vector<T>& vector<T>::operator=(vector& rhs) {
        if(this->operator!=(rhs)){
            const auto len = rhs.size();
            if(this->capasity() < len){
                vector temp(rhs.begin(),rhs.end());
                this->swap(temp);
            }
            else if(len <= this->size()){
                auto i = mystl::copy(rhs.begin(), rhs.end(), this->begin_);
                data_allocator::destroy(i, this->end_);
                end_ = begin_ + len;
            }
            else{
                mystl::copy(rhs.begin(), rhs.begin() + this->size(), this->begin_);
                mystl::uninitialized_copy(rhs.begin() + this->size(), rhs.end(), this->end_);
                end_ = begin_ + len;
                cap_ = end_;
            }
        }
        return *this;

    }
    template <class T>
    vector<T>& vector<T>::operator=(vector&& rhs) noexcept{
        data_allocator::destroy(begin_, end_);
        data_allocator::deallocate(begin_, cap_ - begin_);//似乎没有什么作用
        this->begin_ = rhs.begin_;
        this->end_ = rhs.end_;
        this->cap_ = rhs.cap_;
        rhs.begin_ = nullptr;
        rhs.end_ = nullptr;
        rhs.cap_ = nullptr;
        return *this;
    }
    template <class T>
    bool vector<T>::operator==(vector<T> &rhs) {
        return this->size() == rhs.size() && mystl::equal(this->begin_, this->end_, rhs.begin_);
    }
    template <class T>
    bool vector<T>::operator!=(vector<T> &rhs) {
        return (this->size() != rhs.size() || !mystl::equal(this->begin_, this->end_, rhs.begin_));
    }

//    template <class T>
//    bool operator==(const vector<T>& lhs, const vector<T>& rhs)
//    {
//        return lhs.size() == rhs.size() &&
//               mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
//    }
//
//    template <class T>
//    bool operator!=(const vector<T>& lhs, const vector<T>& rhs)
//    {
//        return !(lhs == rhs);
//    }
}

#endif //MYTINYSTL_VECTOR_H
