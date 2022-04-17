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
        //template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
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
        bool operator==(vector& rhs);
        bool operator!=(vector& rhs);
        vector& operator=(vector& rhs);
        vector& operator=(vector&& rhs) noexcept;
        vector& operator=(std::initializer_list<value_type> initlist);
        void swap(vector& rhs) noexcept;
        ~vector();
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
        const_iterator cbegin() const noexcept{
            return begin();
        }
        const_iterator cend() const noexcept{
            return end();
        }

        //操作
        bool empty() const noexcept{
            return begin_ == end_;
        }
        size_type size() const noexcept{
            return static_cast<size_type>(end_ - begin_);
        }
        size_type max_size() const noexcept{
            return static_cast<size_type>(-1) / sizeof(T);
        }
        size_type capasity() const noexcept{
            return static_cast<size_type>(cap_ - begin_);
        }
        void reserve(size_type n);
        void shrink_to_fit();

        reference operator[](size_type n){
            MYSTL_DEBUG(n < size());
            return *(begin_ + n);
        }
        const_reference operator[](size_type n) const{
            MYSTL_DEBUG(n < size());
            return *(begin_ + n);
        }
        reference at(size_type n){
            THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
            return (*this)[n];
        }
        const reference at(size_type n) const{
            THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
            return (*this)[n];
        }
        reference front(){
            MYSTL_DEBUG(!this->empty());
            return *begin_;
        }
        const_reference front() const{
            MYSTL_DEBUG(!this->empty());
            return *begin_;
        }
        reference back(){
            MYSTL_DEBUG(!empty());
            return *(end_ - 1);
        }
        const_reference back() const{
            MYSTL_DEBUG(!empty());
            return *(end_ - 1);
        }
        pointer data() noexcept{
            return begin_;
        }
        const_pointer data() const noexcept{
            return begin_;
        }
        void assign(size_type n, const value_type& value);
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        void assign(Iter first, Iter last){


        }
        void assign(std::initializer_list<value_type> il);
        template <class... Args>
        iterator emplace(const_iterator pos, Args&& ...args);
        template <class... Args>
        void emplace_back(Args&& ...args);
        void push_back(const value_type& value);
        void push_back(value_type&& value){
            emplace_back(mystl::move(value));
        }
        void pop_back();

        iterator insert(const_iterator pos, const value_type& value);
        iterator insert(const_iterator pos, value_type&& value);
        iterator insert(const_iterator pos, size_type n, const value_type& value);
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        void insert(const_iterator pos, Iter first, Iter last);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        void clear();
        void resize(size_type new_size);
        void resize(size_type new_size, const value_type& value);
        void reverse();





    private:
        void init() noexcept;
        void init_space(size_type size, size_type cap_);
        void fill_init(size_type n, const value_type& value);

        template<class Iter>
        void range_init(Iter first, Iter last);

        size_type get_new_cap(size_type add_size);
        void fill_assign(size_type n, const value_type& value);
        template <class IIter>
        void copy_assign(IIter first, IIter last, input_iterator_tag);
        template <class FIter>
        void copy_assign(FIter first, FIter last, forward_iterator_tag);
        // reallocate

        template <class... Args>
        void reallocate_emplace(iterator pos, Args&& ...args);
        void reallocate_insert(iterator pos, const value_type& value);

        // insert

        iterator fill_insert(iterator pos, size_type n, const value_type& value);
        template <class IIter>
        void copy_insert(iterator pos, IIter first, IIter last);

        // shrink_to_fit

        void reinsert(size_type size);
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
    bool vector<T>::operator==(vector<T> &rhs) {
        return this->size() == rhs.size() && mystl::equal(this->begin_, this->end_, rhs.begin_);
    }
    template <class T>
    bool vector<T>::operator!=(vector<T> &rhs) {
        return (this->size() != rhs.size() || !mystl::equal(this->begin_, this->end_, rhs.begin_));
    }
    template <class T>
    void vector<T>::swap(vector<T> &rhs) noexcept{
        if(this != &rhs){
            mystl::swap(this->begin_, rhs.begin_);
            mystl::swap(this->end_, rhs.end_);
            mystl::swap(this->cap_, rhs.cap_);
        }

    }
    template <class T>
    vector<T>& vector<T>::operator=(vector& rhs)
    {
        if (this != &rhs)
        {
            const auto len = rhs.size();
            if (len > capasity())
            {
                vector tmp(rhs.begin(), rhs.end());
                swap(tmp);
            }
            else if (size() >= len)
            {
                auto i = mystl::copy(rhs.begin(), rhs.end(), begin());
                data_allocator::destroy(i, end_);
                end_ = begin_ + len;
            }
            else
            {
                mystl::copy(rhs.begin(), rhs.begin() + size(), begin_);
                mystl::uninitialized_copy(rhs.begin() + size(), rhs.end(), end_);
                cap_ = end_ = begin_ + len;
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
    vector<T>& vector<T>::operator=(std::initializer_list<value_type> initlist){
        vector temp(initlist.begin(), initlist.end());
        this->swap(temp);
        return *this;
    }
    template <class T>
    vector<T>::~vector(){
        data_allocator::destroy(begin_, end_);
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = nullptr;
        end_ = nullptr;
        cap_ = nullptr;
    }
    template <class T>
    void vector<T>::reserve(size_type n){
        THROW_LENGTH_ERROR_IF(n > max_size(), "n can not larger than max_size() in vector<T>::reserve(n)");
        const size_type old_size = this->size();
        iterator temp = data_allocator::allocate(n);
        mystl::uninitialized_move(this->begin_, this->end_, temp);
        data_allocator::deallocate(this->begin_, this->cap_ - this->begin_);
        this->begin_ = temp;
        this->end_ = temp + old_size;
        this->cap_ = temp + n;
    }
    template <class T>
    void vector<T>::assign(size_type n, const value_type& value){
        fill_init(n,value);

    }
    template <class T>
    void vector<T>::push_back(const value_type& value){
        if(cap_ != end_){
            data_allocator::construct(end_,value);
            end_++;
        }
        else{
            reallocate_insert(end_, value);
        }
    }
    template <class T>
    void vector<T>::pop_back(){
        MYSTL_DEBUG(!empty());
        data_allocator::destroy(end_ - 1);
        end_--;
    }

    template <class T>
    //reallocate_insert
    void vector<T>::reallocate_insert(iterator pos, const value_type& value){
        const size_type new_size = get_new_cap(1);
        iterator new_begin = data_allocator::allocate(new_size);
        iterator new_end = new_begin;
        const value_type& value_copy = value;
        try{
            new_end = mystl::uninitialized_move(begin_, pos, new_begin);
            data_allocator::construct(new_end, value_copy);
            new_end++;
            new_end = mystl::uninitialized_move(pos, end_, new_end);
        }
        catch (...){
            data_allocator::deallocate(new_begin, new_size);
            throw;
        }
        data_allocator::destroy(begin_, end_);
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = new_begin;
        end_ = new_end;
        cap_ = new_begin + new_size;
    }
    template <class T>
    typename vector<T>::size_type
    vector<T>::get_new_cap(size_type add_size)
    {
        const auto old_size = capasity();
        THROW_LENGTH_ERROR_IF(old_size > max_size() - add_size,
                              "vector<T>'s size too big");
        if (old_size > max_size() - old_size / 2)
        {
            return old_size + add_size > max_size() - 16
                   ? old_size + add_size : old_size + add_size + 16;
        }
        const size_type new_size = old_size == 0
                                   ? mystl::max(add_size, static_cast<size_type>(16))
                                   : mystl::max(old_size + old_size / 2, old_size + add_size);
        return new_size;
    }
    template <class T>
    template <class IIter>
    void vector<T>::copy_assign(IIter first, IIter last, input_iterator_tag){
        iterator cur = begin_;
        for(;first != last && cur != end_ ;first++, cur++){
            *cur = *first;
        }
        if(first == last){
            erase(cur, end_);
        }
        else{
            insert(end_, first, last);
        }

    }
    template <class T>
    typename vector<T>::iterator vector<T>::erase(const_iterator pos){
        MYSTL_DEBUG(pos >= begin_ && pos < end_);
        mystl::move(pos + 1, end_, pos);
        end_--;
        return pos;
    }
    template <class T>
    typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last){
        MYSTL_DEBUG(first >= begin() && last <= end() && !(last < first));
        const auto n = first - begin_;
        iterator move_end = mystl::move(last, end_, first);
        data_allocator::destroy(move_end, end_);
        end_ = end_ + (first - last);
        return begin_ + n;

    }

    template <class T>
    template <class... Args>
    void vector<T>::emplace_back(Args&& ...args){
        if(end_ < cap_){
            data_allocator::construct(end_, mystl::forward<Args>(args)...);
            end_++;
        }
        else{
            reallocate_emplace(end_, mystl::forward<Args>(args)...);

        }
    }
    template <class T>
    template <class ...Args>
    void vector<T>::reallocate_emplace(iterator pos, Args&& ...args){
        const size_type new_size = get_new_cap(1);
        iterator new_begin = data_allocator::allocate(new_size);
        iterator new_end = new_begin;
        try{
            new_end = mystl::uninitialized_move(begin_, pos, new_begin);
            data_allocator::construct(new_end, mystl::forward<Args>(args)...);
            new_end++;
            new_end = mystl::uninitialized_move(pos, end_, new_end);
        }
        catch (...){
            data_allocator::deallocate(new_begin, new_size);
            throw;
        }
        data_allocator::destroy(begin_, end_);
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = new_begin;
        end_ = new_end;
        cap_ = new_begin + new_size;
    }
    template <class T>
    typename vector<T>::iterator vector<T>::insert(const_iterator pos, const value_type& value){
        MYSTL_DEBUG(pos >= begin() && pos <= end());
        const size_type n = pos - begin_;
        if(end_ != cap_ && pos == end_){
            data_allocator::construct(end_, value);
            end_++;
        }
        else if(end_ != cap_){
            data_allocator::construct(end_, *(end_ - 1));
            mystl::copy_back(pos, end_ -1, end_);
            *pos = value;
            end_++;
        }
        else{
            reallocate_insert(pos, value);
        }
        return begin_ + n;

    }
    template <class T>
    typename vector<T>::iterator vector<T>::insert(const_iterator pos, value_type&& value){

    }



}

#endif //MYTINYSTL_VECTOR_H
