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
#include "algo.h"


namespace mystl{
    template <class T>
    class vector {
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

        typedef value_type *iterator;
        typedef const value_type *const_iterator;
        typedef mystl::reverse_iterator<iterator> reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

        allocator_type get_allocator(){
            return data_allocator();
        }
    private:
        iterator begin_;
        iterator end_;
        iterator cap_;
    public:
        vector(){
            try_init();
        }
        explicit vector(size_type n){
            fill_init(n, value_type());
        }
        vector(size_type n, const value_type& value){
            fill_init(n, value);
        }
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        vector(Iter first, Iter last){
            range_init(first, last);
        }
        vector(const vector& rhs){
            range_init(rhs.begin(), rhs.end());
        }
        vector(vector&& rhs) noexcept : begin_(rhs.begin_), end_(rhs.end_), cap_(rhs.cap_){
            rhs.begin_ = nullptr;
            rhs.end_ = nullptr;
            rhs.cap_ = nullptr;
        }
        vector(std::initializer_list<value_type> ilist){
            range_init(ilist.begin(), ilist.end());
        }
        vector& operator=(const vector& rhs);
        vector& operator=(vector&& rhs) noexcept;
        vector& operator=(std::initializer_list<value_type> ilist){
            vector temp(ilist.begin(), ilist.end());
            swap(temp);
            return *this;
        }
        ~vector(){
            data_allocator::destroy(begin_, end_);
            data_allocator::deallocate(begin_, cap_ - begin_);
            begin_ = end_ = cap_ = nullptr;
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
        const_iterator end() const noexcept{
            return end_;
        }
        reverse_iterator rbegin() noexcept{
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const noexcept{
            return const_reverse_iterator(end());
        }
        reverse_iterator rend() noexcept{
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const noexcept{
            return const_reverse_iterator(begin());
        }
        const_iterator cbegin() const noexcept{
            return begin();
        }
        const_iterator cend() const noexcept{
            return end();
        }
        const_reverse_iterator crbegin() const noexcept{
            return rbegin();
        }
        const_reverse_iterator crend()const noexcept{
            return rend();
        }

        bool empty() const noexcept{
            return begin_ == end_;
        }
        size_type size() const noexcept{
            return static_cast<size_type>(end_ - begin_);
        }
        size_type capacity() const noexcept{
            return static_cast<size_type>(cap_ - begin_);
        }
        size_type max_size() const noexcept{
            return static_cast<size_type>(-1) / sizeof(T);
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
        const_reference at(size_type n) const{
            THROW_OUT_OF_RANGE_IF(!(n < size()), "vector<T>::at() subscript out of range");
            return (*this)[n];
        }
        reference front(){
            MYSTL_DEBUG(!empty());
            return *begin_;
        }
        const_reference front() const{
            MYSTL_DEBUG(!empty());
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
            MYSTL_DEBUG(!(last < first));
            copy_assign(first, last, iterator_category(first));
        }
        void assign(std::initializer_list<value_type> ilist){
            copy_assign(ilist.begin(), ilist.end(), mystl::forward_iterator_tag{});
        }

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
        iterator insert(const_iterator pos, value_type&& value){
            return emplace(pos, mystl::move(value));
        }

        iterator insert(const_iterator pos, size_type n, const value_type& value);

        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        void insert(const_iterator pos, Iter first, Iter last){
            MYSTL_DEBUG(pos >= begin() && pos <= end() && !(last < first));
            copy_insert(const_cast<iterator>(pos), first, last);
        }



        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        void clear(){
            erase(begin(), end());
        }
        void resize(size_type new_size) {
            resize(new_size, value_type());
        }
        void resize(size_type new_size, const value_type& value);
        void reverse(){
            mystl::reverse(begin(), end());
        }
        void swap(vector<T>& rhs) noexcept;



    private:
        void try_init() noexcept;
        void fill_init(size_type n, const value_type& value);

        template<class Iter>
        void range_init(Iter first, Iter last);


        template<class InputIter>
        void copy_assign(InputIter first, InputIter last, input_iterator_tag);
        template <class ForwardIter>
        void copy_assign(ForwardIter first, ForwardIter last, forward_iterator_tag);
        //size_type get_new_cap(size_type add_size);
        template <class ...Args>
        void reallocate_emplace(iterator pos, Args&& ...args);
        void reallocate_insert(iterator pos, const value_type& value);
        size_type expand(size_type add_size);
        template <class InputIter>
        void copy_insert(iterator pos, InputIter first, InputIter last);







    };
    template <class T>
    void vector<T>::try_init() noexcept{
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
    void vector<T>::fill_init(size_type n, const value_type& value){
        const size_type init_cap = mystl::max(static_cast<size_type>(16), n);
        try{
            begin_ = data_allocator::allocate(init_cap);
            end_ = begin_ + n;
            cap_ = begin_ + init_cap;
        }
        catch (...){
            begin_ = nullptr;
            end_ = nullptr;
            cap_ = nullptr;
            throw;
        }
        mystl::uninitialized_fill_n(begin_, n, value);
    }
    template <class T>
    template <class Iter>
    void vector<T>::range_init(Iter first, Iter last){
        const size_type init_cap = mystl::max(static_cast<size_type>(16), static_cast<size_type>(last - first));
        try{
            begin_ = data_allocator::allocate(init_cap);
            end_ = begin_ + static_cast<size_type>(last - first);
            cap_ = begin_ + init_cap;
        }
        catch (...){
            begin_ = nullptr;
            end_ = nullptr;
            cap_ = nullptr;
            throw;
        }
        mystl::uninitialized_copy(first, last, begin_);
    }
    template <class T>
    vector<T>& vector<T>::operator=(const vector& rhs){
        if(this != &rhs){
            const auto n = rhs.size();
            if(capacity() < n){
                vector temp(rhs.begin(), rhs.end());
                swap(temp);
            }
            else if(n <= size()){
                iterator new_end = mystl::copy(rhs.begin(), rhs.end(), begin_);
                data_allocator::destroy(new_end, end_);
                end_ = new_end;
            }
            else{
                mystl::copy(rhs.begin(), rhs.begin() + size(), begin_);
                mystl::uninitialized_copy(rhs.begin() + size(), rhs.end(), end_);
                cap_ = end_ = begin_ + n;
            }
        }
        return *this;
    }
    template <class T>
    void vector<T>::swap(vector<T>& rhs) noexcept{
        if(this != &rhs){
            mystl::swap(begin_, rhs.begin_);
            mystl::swap(end_, rhs.end_);
            mystl::swap(cap_, rhs.cap_);
        }
    }
    template <class T>
    vector<T>& vector<T>::operator=(vector&& rhs) noexcept{
        data_allocator::destroy(begin_, end_);
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = rhs.begin_;
        end_ = rhs.end_;
        cap_ = rhs.end_;
        rhs.begin_ = nullptr;
        rhs.end_ = nullptr;
        rhs.cap_ = nullptr;
        return *this;
    }
    template <class T>
    void vector<T>::reserve(size_type n){
        if(capacity() < n){
            const size_type old_size = size();
            auto new_begin = data_allocator::allocate(n);
            mystl::uninitialized_move(begin_, end_, new_begin); //move?????????????????????destroy
            data_allocator::destroy(begin_, end_);
            data_allocator::deallocate(begin_, cap_ - begin_);
            begin_ = new_begin;
            end_ = new_begin + old_size;
            cap_ = new_begin + n;
        }
    }
    template <class T>
    void vector<T>::shrink_to_fit(){
        if(end_ < cap_){
            iterator new_begin = data_allocator::allocate(size());
            try{
                mystl::uninitialized_move(begin_, end_, new_begin);
            }
            catch (...){
                data_allocator::deallocate(new_begin, size());
                throw ;
            }
            data_allocator::deallocate(begin_, cap_ - begin_);
            begin_ = new_begin;
            cap_ = end_ = begin_ + size();
        }

    }
    template <class T>
    void vector<T>::assign(size_type n, const value_type& value){
        if(capacity() < n){
            vector temp(n, value);
            swap(temp);
        }
        else if(size() < n){
            mystl::fill(begin_, end_, value);
            end_ = mystl::uninitialized_fill_n(end_, n - size(), value);
        }
        else{
            iterator new_end = mystl::fill_n(begin_, n, value);
            data_allocator::destroy(new_end, end_);
            end_ = new_end;
        }
    }
    template <class T>
    template <class InputIter>
    void vector<T>::copy_assign(InputIter first, InputIter last, input_iterator_tag){
        auto cur = begin_;
        for(;cur != end_ && first != last; ++first, ++cur){
            *first = *cur;
        }
        if(first == last){
            erase(cur, end_);
        }
        else{
            insert(end_,first,last);
        }
    }
    template <class T>
    template <class ForwardIter>
    void vector<T>::copy_assign(ForwardIter first, ForwardIter last, forward_iterator_tag){
        const size_type n = mystl::distance(first,last);
        if(capacity() < n){
            vector temp(first, last);
            swap(temp);
        }
        else if(n <= size()){
            auto new_end = mystl::copy(first,last,begin_);
            data_allocator::destroy(new_end,end_);
            end_ = new_end;
        }
        else{
            auto mid = first;
            mystl::advance(mid, size());
            mystl::copy(first, mid, begin_);
            auto new_end = mystl::uninitialized_copy(mid, last, end_);
            end_ = new_end;
        }
    }
    template <class T>
    template <class ...Args>
    typename vector<T>::iterator
    vector<T>::emplace(const_iterator pos, Args&& ...args){
        MYSTL_DEBUG(pos >= begin() && pos <= end());
        iterator xpos = const_cast<iterator>(pos);
        const size_type n = xpos - begin_;
        if(end_ != cap_ && pos == end_){
            data_allocator::construct(mystl::address_of(*end_), mystl::forward<Args>(args)...);
            ++end_;
        }
        else if(end_ != cap_){
            iterator new_end = end_;
            data_allocator::construct(mystl::address_of(*end_), *(end_ - 1));
            ++new_end;
            mystl::copy_back(xpos, end_ - 1, end_);
            *xpos = value_type(mystl::forward<Args>(args)...);
            end_ = new_end;
        }
        else{
            reallocate_emplace(xpos, mystl::forward<Args>(args)...);
        }
        return begin() + n;
    }
    template <class T>
    template <class... Args>
    void vector<T>::emplace_back(Args&& ...args){
        if(end_ < cap_){
            data_allocator::construct(mystl::address_of(*end_), mystl::forward<Args>(args)...);
            ++end_;
        }
        else{
            reallocate_emplace(end_, mystl::forward<Args>(args)...);
        }
    }
    template <class T>
    template <class ...Args>
    void vector<T>::reallocate_emplace(iterator pos, Args&& ...args){
        const auto new_size = expand(1);
        auto new_begin = data_allocator::allocate(new_size);
        auto new_end = new_begin;
        try{
            new_end = mystl::uninitialized_move(begin_, pos, new_begin);
            data_allocator::construct(mystl::address_of(*new_end), mystl::forward<Args>(args)...);
            ++new_end;
            new_end = mystl::uninitialized_move(pos, end_, new_end);
        }
        catch (...) {
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
    typename vector<T>::size_type vector<T>::expand(size_type add_size){
        const size_type new_cap = size() + add_size <=  2 * capacity() ? 2 * capacity() : size() + add_size;
        return new_cap;
    }
    template <class T>
    void vector<T>::push_back(const value_type& value){
        if(end_ < cap_){
            data_allocator::construct(mystl::address_of(*end_), value);
            ++end_;
        }
        else{
            reallocate_insert(end_, value);
        }


    }
    template <class T>
    void vector<T>::reallocate_insert(iterator pos, const value_type& value){
        const auto new_size = expand(1);
        auto new_begin = data_allocator::allocate(new_size);
        auto new_end = new_begin;
        const value_type& value_copy = value;
        try{
            new_end = mystl::uninitialized_move(begin_, pos, new_begin);
            data_allocator::construct(mystl::address_of(*new_end), value_copy);
            ++new_end;
            new_end = mystl::uninitialized_move(pos, end_, new_end);
        }
        catch (...){
            data_allocator::deallocate(new_begin, new_size);
            throw;
        }
        destroy_and_recover(begin_, end_, cap_ - begin_);
        begin_ = new_begin;
        end_ = new_end;
        cap_ = new_begin + new_size;
    }
    template <class T>
    void vector<T>::pop_back(){
        MYSTL_DEBUG(!empty());
        data_allocator::destroy(end_ - 1);
        --end_;
    }
    template <class T>
    typename vector<T>::iterator vector<T>::insert(const_iterator pos, const value_type& value){
        MYSTL_DEBUG(pos >= begin() && pos <= end());
        iterator xpos = const_cast<iterator>(pos);
        const size_type n = xpos - begin_;
        if(end_ != cap_ && pos == end_){
            data_allocator::construct(mystl::address_of(*end_), value);
            ++end_;
        }
        else if(end_ != cap_){
            iterator new_end = end_;
            data_allocator::construct(mystl::address_of(*end_), *(end_ - 1));
            ++new_end;
            auto value_copy = value;
            mystl::copy_back(xpos, end_ - 1, end_);
            *xpos = mystl::move(value_copy);
            end_ = new_end;
        }
        else{
            reallocate_insert(xpos, value);
        }
        return begin_ + n;
    }
    template <class T>
    typename vector<T>::iterator vector<T>::insert(const_iterator pos, size_type n, const value_type& value) {
        MYSTL_DEBUG(pos >= begin() && pos <= end());
        if (n == 0) {
            return pos;
        }
        const size_type diff = pos - begin_;
        const value_type value_copy = value;
        if (static_cast<size_type>(cap_ - end_) >= n) {
            const size_type nElems = end_ - pos;
            auto old_end = end_;
            if (nElems > n) {
                mystl::uninitialized_copy(end_ - n, end_, end_);
                end_ += n;
                mystl::move_backward(pos, old_end - n, old_end);
                mystl::uninitialized_fill_n(pos, n, value_copy);
            } else {
                end_ = mystl::uninitialized_fill_n(end_, n - nElems, value_copy);
                end_ = mystl::uninitialized_move(pos, old_end, end_);
                mystl::uninitialized_fill_n(pos, nElems, value_copy);
            }
        }
        else {
            const auto new_size = expand(n);
            auto new_begin = data_allocator::allocate(new_size);
            auto new_end = new_begin;
            try {
                new_end = mystl::uninitialized_move(begin_, pos, new_begin);
                new_end = mystl::uninitialized_fill_n(new_end, n, value);
                new_end = mystl::uninitialized_move(pos, end_, new_end);
            }
            catch (...) {
                destroy_and_recover(new_begin, new_end, new_size);
                throw;
            }
            data_allocator::destroy(begin_, end_);
            data_allocator::deallocate(begin_, cap_ - begin_);
            begin_ = new_begin;
            end_ = new_end;
            cap_ = begin_ + new_size;
        }
        return begin_ + diff;
    }
    template <class T>
    template <class InputIter>
    void vector<T>::copy_insert(iterator pos, InputIter first, InputIter last){
        if(first == last){
            return;
        }
        const auto  n = mystl::distance(first, last);
        if((cap_ - end_) >= n){
            const auto after_elems = end_ - pos;
            auto old_end = end_;
            if (after_elems > n){
                end_ = mystl::uninitialized_copy(end_ - n, end_, end_);
                mystl::move_backward(pos, old_end - n, old_end);
                mystl::uninitialized_copy(first, last, pos);
            }
            else{
                auto mid = first;
                mystl::advance(mid, after_elems);
                end_ = mystl::uninitialized_copy(mid, last, end_);
                end_ = mystl::uninitialized_move(pos, old_end, end_);
                mystl::uninitialized_copy(first, mid, pos);
            }
        }
        else{
            const auto new_size = get_new_cap(n);
            auto new_begin = data_allocator::allocate(new_size);
            auto new_end = new_begin;
            try{
                new_end = mystl::uninitialized_move(begin_, pos, new_begin);
                new_end = mystl::uninitialized_copy(first, last, new_end);
                new_end = mystl::uninitialized_move(pos, end_, new_end);
            }
            catch (...){
                destroy_and_recover(new_begin, new_end, new_size);
                throw;
            }
            data_allocator::deallocate(begin_, cap_ - begin_);
            begin_ = new_begin;
            end_ = new_end;
            cap_ = begin_ + new_size;
        }
    }
    template <class T>
    typename vector<T>::iterator vector<T>::erase(const_iterator pos){
        MYSTL_DEBUG(pos >= begin() && pos < end());
        iterator xpos = begin_ + pos - begin();
        mystl::move(xpos + 1, end_, xpos);
        data_allocator::destroy(end_ - 1);
        --end_;
        return xpos;
    }
    template <class T>
    typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last){
        MYSTL_DEBUG(first >= begin() && last <= end() && !(last < first));
        const auto n = first - begin();
        iterator r = begin_ + (first - begin());
        data_allocator::destroy(mystl::move(r + (last - first), end_, r), end_);
        end_ = end_ - (last - first);
        return begin_ + n;
    }
    template <class T>
    void vector<T>::resize(size_type new_size, const value_type& value){
        if(new_size < size()){
            erase(begin_ + new_size, end_);
        }
        else{
            insert(end_, new_size - size(), value);
        }
    }

    template <class T>
    bool operator==(const vector<T>& lhs, const vector<T>& rhs){
        return lhs.size() == rhs.size() &&
               mystl::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class T>
    bool operator<(const vector<T>& lhs, const vector<T>& rhs){
        return mystl::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
    }

    template <class T>
    bool operator!=(const vector<T>& lhs, const vector<T>& rhs){
        return !(lhs == rhs);
    }

    template <class T>
    bool operator>(const vector<T>& lhs, const vector<T>& rhs){
        return rhs < lhs;
    }

    template <class T>
    bool operator<=(const vector<T>& lhs, const vector<T>& rhs){
        return !(rhs < lhs);
    }

    template <class T>
    bool operator>=(const vector<T>& lhs, const vector<T>& rhs){
        return !(lhs < rhs);
    }

    template <class T>
    void swap(vector<T>& lhs, vector<T>& rhs){
        lhs.swap(rhs);
    }


}

#endif //MYTINYSTL_VECTOR_H
