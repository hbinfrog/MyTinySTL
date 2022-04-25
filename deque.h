//
// Created by hbin on 2022/4/19.
//

#ifndef MYTINYSTL_DEQUE_H
#define MYTINYSTL_DEQUE_H
#include <initializer_list>
#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl{
    #ifndef DEQUE_MAP_INIT_SIZE
    #define DEQUE_MAP_INIT_SIZE 8
    #endif
    template <class T>
    struct deque_buf_size
    {
        static constexpr size_t value = 8; //sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
    };
    template <class T, class Ref, class Ptr>
    struct deque_iterator : public iterator<random_access_iterator_tag, T>{
        typedef deque_iterator<T, T&, T*>             iterator;
        typedef deque_iterator<T, const T&, const T*> const_iterator;
        typedef deque_iterator                        self;
        typedef T            value_type;
        typedef Ptr          pointer;
        typedef Ref          reference;
        typedef size_t       size_type;
        typedef ptrdiff_t    diff_type;
        typedef T*           value_pointer;
        typedef T**          map_pointer;
        static const size_type buffer_size = deque_buf_size<T>::value;

        value_pointer cur;    // 指向所在缓冲区的当前元素
        value_pointer first;  // 指向所在缓冲区的头部
        value_pointer last;   // 指向所在缓冲区的尾部
        map_pointer   node;   // 缓冲区所在节点
        deque_iterator() noexcept : cur(nullptr), first(nullptr), last(nullptr), node(nullptr){}
        deque_iterator(value_pointer v, map_pointer n) : cur(v), first(*n), last(*n + buffer_size), node(n){}
        deque_iterator(const iterator& rhs) : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}

        deque_iterator(iterator&& rhs) noexcept : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){
            rhs.cur = nullptr;
            rhs.first = nullptr;
            rhs.last = nullptr;
            rhs.node = nullptr;
        }
        deque_iterator(const const_iterator& rhs) : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}
        self& operator=(const iterator& rhs){
            if(this != &rhs){
                cur = rhs.cur;
                first = rhs.first;
                last = rhs.last;
                node = rhs.node;
            }
            return *this;
        }
        void set_node(map_pointer new_node){
            node = new_node;
            first = *node;
            last = first + buffer_size ;//cur????
        }
        reference operator*(){
            return *cur;
        }
        pointer operator->(){
            return cur;
        }
        diff_type operator-(const self& x) const{
            return static_cast<diff_type>(buffer_size) * (node - x.node) + (cur - first) - (x.cur - x.first);
        }
        self& operator++(){
            ++cur;
            if(cur == last){
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }
        self operator++(int){
            self temp = *this;
            ++*this;
            return temp;
        }
        self& operator--(){
            if(cur == first){
                set_node(node - 1);
                cur = last;
            }
            --cur;
            return *this;
        }
        self operator--(int){
            self temp = *this;
            --*this;
            return *this;
        }
        self& operator+=(diff_type n){
            const diff_type offset = n + (cur - first);
            if(offset >= 0 && offset < static_cast<diff_type>(buffer_size)){
                cur += n;
            }
            else{
                diff_type node_offset = 0;
                if(offset < 0){
                    node_offset = -(-offset - 1) / diff_type(buffer_size) - 1;
                }
                else{
                    node_offset = offset / diff_type(buffer_size);
                }
                set_node(node + node_offset);
                cur = first + (offset - node_offset * static_cast<diff_type>(buffer_size));
            }
            return *this;

        }
        self operator+(diff_type n) const{
            self temp = *this;
            return temp += n;
        }
        self& operator-=(diff_type n){
            return *this += -n;
        }
        self operator-(diff_type n) const{
            self temp = *this;
            return temp -= n;
        }
        bool operator==(const self& rhs) const{
            return cur == rhs.cur;
        }
        bool operator< (const self& rhs) const{
            if(node == rhs.node){
                return cur < rhs.cur;
            }
            else{
                return node < rhs.node;
            }
        }
        bool operator!=(const self& rhs) const{
            return !(*this == rhs);
        }
        bool operator>(const self& rhs) const{
            return rhs < *this;
        }
        bool operator<=(const self& rhs) const{
            return !(*this > rhs);
        }
        bool operator>=(const self& rhs) const{
            return !(*this < rhs);
        }
    };
    template <class T>
    class deque{
    public:
        typedef mystl::allocator<T> allocator_type;
        typedef mystl::allocator<T> data_allocator;
        typedef mystl::allocator<T*> map_allocator;
        typedef typename allocator_type::value_type value_type;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::diff_type diff_type;
        typedef pointer* map_pointer;
        typedef const_pointer* const_map_pointer;
        typedef deque_iterator<T, T&, T*> iterator;
        typedef deque_iterator<T, const T&, const T*> const_iterator;
        typedef mystl::reverse_iterator<iterator> reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator> const_reverse_iterator;
        allocator_type get_allocator(){
            return allocator_type();
        }
        static const size_type buffer_size = deque_buf_size<T>::value;
    private:
        iterator begin_;
        iterator end_;
        map_pointer map_;
        size_type map_size_;

    public:
        deque(){
            fill_init(0, value_type());
        }
        explicit deque(size_type n){
            fill_init(n, value_type());
        }
        deque(size_type n, const value_type& value){
            fill_init(n,value);
        }
        template <class IIter, typename std::enable_if<mystl::is_input_iterator<IIter>::value, int>::type = 0>
        deque(IIter first, IIter last){
            copy_init(first,last,iterator_category(first));
        }
        deque(std::initializer_list<value_type> ilist){
            copy_init(ilist.begin(), ilist.end(), mystl::forward_iterator_tag());
        }
        deque(const deque& rhs){
            copy_init(rhs.begin(), rhs.end(), mystl::forward_iterator_tag());

        }
        deque(deque&& rhs) noexcept : begin_(mystl::move(rhs.begin_)), end_(mystl::move(rhs.end_)), map_(rhs.map_), map_size_(rhs.map_size_){
            rhs.map_ = nullptr;
            rhs.map_size_ = 0;
        }
        deque& operator=(const deque& rhs);
        deque& operator=(deque&& rhs);
        deque& operator=(std::initializer_list<value_type> ilist);
        ~deque(){
            if(map_ != nullptr){
                clear();
                data_allocator::deallocate(*begin_.node, buffer_size);
                *begin_.node = nullptr;
                map_allocator::deallocate(map_, map_size_);
                map_ = nullptr;
            }
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
            return reverse_iterator(end());
        }
        reverse_iterator rend() noexcept{
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const noexcept{
            return reverse_iterator(begin());
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
        const_reverse_iterator crend() const noexcept{
            return rend();
        }
        bool empty(){
            return end_ == begin_;
        }
        size_type size() const noexcept{
            return end_ - begin_;
        }
        size_type max_size() const noexcept{
            return static_cast<size_type>(-1);
        }
        reference operator[](size_type n){
            MYSTL_DEBUG(n < size());
            return *(begin_ + n);
        }
        const_reference operator[](size_type n) const{
            MYSTL_DEBUG(n < size());
            return *(begin_ + n);
        }
        reference at(size_type n){
            THROW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range");
            return (*this)[n];
        }
        const_reference at(size_type n) const{
            THROW_OUT_OF_RANGE_IF(!(n < size()), "deque<T>::at() subscript out of range");
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
        void assign(size_type n, const value_type& value);
        template <class IIter, typename std::enable_if<mystl::is_input_iterator<IIter>::value, int>::type = 0>
        void assign(IIter first, IIter last);
        void assign(std::initializer_list<value_type> ilist);

        template <class ...Args>
        void emplace_front(Args&& ...args);
        template <class ...Args>
        void emplace_back(Args&& ...args);
        template <class ...Args>
        iterator emplace(iterator pos, Args&& ...args);

        void push_front(const value_type& value);
        void push_back(const value_type& value);
        void push_front(value_type&& value){
            emplace_front(mystl::move(value));
        }
        void push_back(value_type&& value){
            emplace_back(mystl::move(value));
        }
        void pop_back();
        void pop_front();

        iterator insert(iterator pos, const value_type& value);
        iterator insert(iterator pos, value_type&& value);
        void insert(iterator pos, size_type n, const value_type& value);
        template <class IIter, typename std::enable_if<mystl::is_input_iterator<IIter>::value, int>::type = 0>
        void insert(iterator pos, IIter first, IIter last);

        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);


        void clear();
        void swap(deque& rhs) noexcept;
    private:
        void map_init(size_type n);
        void fill_init(size_type n, const value_type& value);
        map_pointer create_map(size_type n);
        void create_buffer(map_pointer nstart, map_pointer nfinish);

        template<class InputIter>
        void copy_init(InputIter first, InputIter last, input_iterator_tag);
        template<class FowardIter>
        void copy_init(FowardIter first, FowardIter last, forward_iterator_tag);
        void reallocate_map(size_type add_buffer, bool back);//back == 1表示在后拓展，back == 0表示在前拓展
        template <class... Args>
        iterator insert_aux(iterator pos, Args&& ...args);
    };
    template<class T>
    void deque<T>::map_init(size_type n){
        const size_type numNode = n / buffer_size + 1;
        map_size_ = mystl::max(static_cast<size_type>(DEQUE_MAP_INIT_SIZE), numNode + 2);
        try{
            map_ = create_map(map_size_);
        }
        catch (...){
            map_ = nullptr;
            map_size_ = 0;
            throw;
        }
        map_pointer nstart = map_ + (map_size_ - numNode) / 2;
        map_pointer nfinish = nstart + numNode - 1;
        try
        {
            create_buffer(nstart, nfinish);
        }
        catch (...)
        {
            map_allocator::deallocate(map_, map_size_);
            map_ = nullptr;
            map_size_ = 0;
            throw;
        }
        begin_.set_node(nstart);
        end_.set_node(nfinish);
        begin_.cur = begin_.first;
        end_.cur = end_.first + (n % buffer_size);
    }
    template <class T>
    void deque<T>::fill_init(size_type n, const value_type& value) {
        map_init(n);
        if(n != 0){
            for(auto cur = begin_.node; cur < end_.node; ++cur){
                for(size_type i = 0; i < buffer_size; ++i){
                    *(*cur+i) = value;
                }
            }
            for(auto curr = end_.first; curr != end_.cur; ++curr){
                *curr = value;
            }
        }
    }
    template<class T>
    typename deque<T>::map_pointer deque<T>::create_map(size_type n){
        map_pointer mp = nullptr;
        mp = map_allocator::allocate(n);
        for (size_type i = 0; i < n; i++){
            *(mp + i) = nullptr;
        }
        return mp;
    }
    template<class T>
    void deque<T>::create_buffer(map_pointer nstart, map_pointer nfinish){
        map_pointer cur;
        try{
            for(cur = nstart; cur <= nfinish; ++cur){
                *cur = data_allocator::allocate(buffer_size);
            }
        }
        catch (...){
            while (cur != nstart){
                --cur;
                data_allocator::deallocate(*cur, buffer_size);
                *cur = nullptr;
            }
            throw;
        }

    }
    template <class T>
    template<class InputIter>
    void deque<T>::copy_init(InputIter first, InputIter last, input_iterator_tag){
        const size_type n = mystl::distance(first,last);
        map_init(n);
        for(auto it = begin_; it != end_; ++it){
            *it = *first;
            ++first;
        }// I don't know whether it is right????
    }
    template <class T>
    template <class FowardIter>
    void deque<T>::
    copy_init(FowardIter first, FowardIter last, forward_iterator_tag){
        const size_type n = mystl::distance(first, last);
        map_init(n);
        for(auto cur_ = begin_.node; cur_ < end_.node; ++cur_){
            auto next = first;
            next += buffer_size;
            for(size_type i = 0; i < buffer_size && first != next; i++, ++first){
                *(*cur_ + i) = *first;
            }
            first = next;
        }
        for(auto cur_ = end_.first; cur_ != end_.cur && first != last; ++cur_, ++first){
            *cur_ = *first;
        }

    }
    template <class T>
    deque<T>& deque<T>::operator=(const deque& rhs){
        if(this != &rhs){
            const size_type n = size();
            auto result = begin_;
            if(rhs.size() <= n){
                erase(mystl::copy(rhs.begin_, rhs.end_, begin_), end_);
            }
            else{
                iterator mid = rhs.begin() + static_cast<diff_type>(n);
                mystl::copy(rhs.begin_, mid, begin_);
                insert(end_, mid, rhs.end_);
            }
        }
        return *this;


    }
    template <class T>
    void deque<T>::clear(){
        if(begin_.node == end_.node){
            data_allocator::destroy(begin_.cur, end_.cur);
        }
        else{
            data_allocator::destroy(begin_.cur, begin_.last);
            for(map_pointer cur_ = begin_.node + 1; cur_ < end_.node; ++cur_){
                data_allocator::destroy(*cur_, *cur_ + buffer_size);
            }
            data_allocator::destroy(end_.first, end_.cur);
        }
        for(map_pointer cur_ = map_; cur_ < begin_.node; ++cur_){
            data_allocator::deallocate(*cur_, buffer_size);
            *cur_ = nullptr;
        }
        for(map_pointer cur_ = end_.node + 1; cur_ < map_ + map_size_; ++cur_){
            data_allocator::deallocate(*cur_, buffer_size);
            *cur_ = nullptr;
        }
        end_ = begin_;
    }
    template <class T>
    deque<T>& deque<T>::operator=(deque&& rhs){
        clear();
        begin_ = mystl::move(rhs.begin_);
        end_ = mystl::move(rhs.end_);
        map_ = rhs.map_;
        map_size_ = rhs.map_size_;
        rhs.map_ = nullptr;
        rhs.map_size_ = 0;
    }
    template <class T>
    deque<T>& deque<T>::operator=(std::initializer_list<value_type> ilist){
        deque temp(ilist);
        swap(temp);
        return *this;
    }
    template <class T>
    void deque<T>::swap(deque& rhs) noexcept{
        if(this != &rhs){
            mystl::swap(begin_, rhs.begin_);
            mystl::swap(end_, rhs.end_);
            mystl::swap(map_, rhs.map_);
            mystl::swap(map_size_, rhs.map_size_);
        }
    }


    template <class T>
    void deque<T>::push_front(const value_type& value){
        if(begin_.cur != begin_.first){
            data_allocator::construct(begin_.cur - 1, value);
            --begin_.cur;
        }
        else{
            const size_type add_buffer = 1;
            if(add_buffer > static_cast<size_type>(begin_.node - map_)){
                reallocate_map(add_buffer, false);
            }
            else{
                create_buffer(begin_.node - add_buffer, begin_.node - 1);
            }
            --begin_;
            data_allocator::construct(begin_.cur, value);
        }

    }
    template <class T>
    void deque<T>::push_back(const value_type& value){
        if(end_.cur != end_.last - 1){
            data_allocator::construct(end_.cur, value);
            ++end_.cur;
        }
        else{
            const size_type add_buffer = 1;
            if(add_buffer > static_cast<diff_type>(map_ + map_size_ - end_.node - 1)){
                reallocate_map(add_buffer, true);
            }
            else{
                create_buffer(end_.node + 1, end_.node + add_buffer);
            }
            data_allocator::construct(end_.cur, value);
            ++end_;
        }
    }
    template <class T>
    template <class ...Args>
    void deque<T>::emplace_front(Args&& ...args){
        if(begin_.cur != begin_.first){
            data_allocator::construct(begin_.cur - 1, mystl::forward<Args>(args)...);
            --begin_.cur;
        }
        else{
            const size_type add_buffer = 1;
            if(add_buffer > static_cast<size_type>(begin_.node - map_)){
                reallocate_map(add_buffer, false);
            }
            else{
                create_buffer(begin_.node - add_buffer, begin_.node - 1);
            }
            --begin_;
            data_allocator::construct(begin_.cur, mystl::forward<Args>(args)...);
        }
    }
    template <class T>
    template <class ...Args>
    void deque<T>::emplace_back(Args&& ...args){
        if(end_.cur != end_.last - 1){
            data_allocator::construct(end_.cur, mystl::forward<Args>(args)...);
            ++end_.cur;
        }
        else{
            const size_type add_buffer = 1;
            if(add_buffer > static_cast<diff_type>(map_ + map_size_ - end_.node - 1)){
                reallocate_map(add_buffer, true);
            }
            else{
                create_buffer(end_.node + 1, end_.node + add_buffer);
            }
            data_allocator::construct(end_.cur, mystl::forward<Args>(args)...);
            ++end_;
        }
    }
    template <class T>
    template <class ...Args>
    typename deque<T>::iterator deque<T>::emplace(iterator pos, Args&& ...args){//在pos之前插入
        if(pos.cur == end_.cur){
            emplace_front(mystl::forward<Args>(args)...);
            return begin_;
        }
        else if(pos.cur == end_.cur){
            emplace_back(mystl::forward<Args>(args)...);
            return end_;
        }
        return insert_aux(pos,mystl::forward<Args>(args)...);
    }
    template <class T>
    void deque<T>::reallocate_map(size_type add_buffer, bool back){
        const size_type new_map_size = mystl::max(map_size_ << 1, map_size_ + add_buffer + DEQUE_MAP_INIT_SIZE);
        map_pointer new_map = create_map(new_map_size);
        const size_type old_buffer = end_.node - begin_.node + 1;
        const size_type new_buffer = old_buffer + add_buffer;
        map_pointer begin;
        map_pointer mid;
        map_pointer end;
        if(back){
            begin = new_map + (new_map_size - new_buffer) / 2;
            mid = begin + old_buffer;
            end = mid + add_buffer;
            for (auto begin1 = begin, begin2 = begin_.node; begin1 != mid; ++begin1, ++begin2){
                *begin1 = *begin2;
            }
            create_buffer(mid, end - 1);
            begin_ = iterator(*begin + (begin_.cur - begin_.first), begin);
            end_ = iterator(*(mid - 1) + (end_.cur - end_.first), mid - 1);
        }
        else{
            begin = new_map + (new_map_size - new_buffer) / 2;
            mid = begin + add_buffer;
            end = mid + old_buffer;
            create_buffer(begin, mid - 1);
            for(auto begin1 = mid, begin2 = begin_.node; begin1 != end; ++begin1, ++begin2){
                *begin1 = *begin2;
            }
            begin_ = iterator(*mid + (begin_.cur - begin_.first), mid);
            end_ = iterator(*(end - 1) + (end_.cur - end_.first), end - 1);
        }
        map_allocator::deallocate(map_, map_size_);
        map_ = new_map;
        map_size_ = new_map_size;
    }
    template <class T>
    template <class... Args>
    typename deque<T>::iterator deque<T>::insert_aux(iterator pos, Args&& ...args){
        const size_type diff = pos - begin_;
        value_type args_copy = value_type(mystl::forward<Args>(args)...);
        if(diff < size() / 2){
            emplace_front(front());
            auto front1 = begin_;
            ++front1;
            auto front2 = front1;
            ++front2;
            pos = begin_ + diff;
            auto cur = pos;
            ++cur;
            for(;front2 != cur; ++front2, ++front1){
                *front1 = *front2;
            }
            mystl::copy(front2, pos, front1);
        }
        else{
            emplace_back(back());
            auto back1 = end_;
            --back1;
            auto back2 = back1;
            --back2;
            pos = begin_ + diff;
            for(auto cur_ = pos; cur_ != back2; ++cur_, ++back1){
                *back1 = *pos;
            }
        }
        *pos = mystl::move(args_copy);
        return pos;

    }
    template <class T>
    void deque<T>::pop_front(){
        MYSTL_DEBUG(!empty());
        if(begin_.cur != begin_.last - 1){
            data_allocator::destroy(begin_.cur);
            ++begin_.cur;
        }
        else{
            data_allocator::destroy(begin_.cur);
            ++begin_;
            data_allocator::deallocate(*(begin_.node - 1), buffer_size);
            *(begin_.node - 1) = nullptr;
        }
    }
    template <class T>
    void deque<T>::pop_back(){
        MYSTL_DEBUG(!empty());
        if(end_.cur != end_.first){
            --end_.cur;
            data_allocator::destroy(end_.cur);
        }
        else{
            --end_;
            data_allocator::destroy(end_.cur);
            data_allocator::deallocate(*(end_.node + 1), buffer_size);
            *(end_.node + 1) = nullptr;
        }
    }
    template <class T>
    typename deque<T>::iterator deque<T>::insert(iterator pos, const value_type& value){
        if(pos.cur == begin_.cur){
            push_front(value);
            return begin_;
        }
        else if(pos.cur == end_.cur){//加else，可能出现begin_ == end_的情况
            push_back(value);
            return (end_ - 1);
        }
        else{
            return insert_aux(pos,value);
        }
    }
    template <class T>
    typename deque<T>::iterator deque<T>::insert(iterator pos, value_type&& value){
        if(pos.cur == begin_.cur){
            push_front(mystl::move(value));
            return begin_;
        }
        else if(pos.cur == end_.cur){//加else，可能出现begin_ == end_的情况
            push_back(mystl::move(value));
            return (end_ - 1);
        }
        else{
            return insert_aux(pos,mystl::move(value));
        }
    }
    template <class T>
    void deque<T>::insert(iterator pos, size_type n, const value_type& value){
        if(pos.cur == begin_.cur){
            if(static_cast<size_type>(begin_.cur - begin_.first) < n){
                const size_type add_buffer = (n - (begin_.cur - begin_.first)) / buffer_size + 1;
                if (add_buffer > static_cast<size_type>(begin_.node - map_))
                {
                    reallocate_map(add_buffer, false);
                }
                else{
                    create_buffer(begin_.node - add_buffer, begin_.node - 1);
                }
            }
            auto new_begin = begin_ - n;
            mystl::uninitialized_fill_n(new_begin, n, value);
            begin_ = new_begin;
        }
        else if(pos.cur == end_.cur){
            if(static_cast<size_type>(end_.last - end_.cur - 1) < n){
                const size_type add_buffer = (n - (end_.last - end_.cur - 1)) / buffer_size + 1;
                if (add_buffer > static_cast<size_type>((map_ + map_size_) - end_.node - 1))
                {
                    reallocate_map(add_buffer, true);
                }
                else{
                    create_buffer(end_.node + 1, end_.node + add_buffer);
                }

            }
            auto new_end = end_ + n;
            mystl::uninitialized_fill_n(end_, n, value);
            end_ = new_end;
        }
        else{
            const size_type diff = pos - begin_;
            const size_type len = size();
            value_type value_copy = value;
            if(diff < len / 2){
                if(static_cast<size_type>(begin_.cur - begin_.first) < n){
                    const size_type add_buffer = (n - (begin_.cur - begin_.first)) / buffer_size + 1;
                    if (add_buffer > static_cast<size_type>(begin_.node - map_))
                    {
                        reallocate_map(add_buffer, false);
                    }
                    else{
                        create_buffer(begin_.node - add_buffer, begin_.node - 1);
                    }

                }
                auto old_begin = begin_;
                auto new_begin = begin_ - n;
                pos = begin_ + diff;
                if (diff >= n)
                {
                    auto begin_n = begin_ + n;
                    mystl::uninitialized_copy(begin_, begin_n, new_begin);
                    begin_ = new_begin;
                    mystl::copy(begin_n, pos, old_begin);
                    mystl::fill(pos - n, pos, value_copy);
                }
                else
                {
                    mystl::uninitialized_fill(
                            mystl::uninitialized_copy(begin_, pos, new_begin), begin_, value_copy);
                    begin_ = new_begin;
                    mystl::fill(old_begin, pos, value_copy);
                }
            }
            else{
                if(static_cast<size_type>(begin_.cur - begin_.first) < n){
                    const size_type add_buffer = (n - (end_.last - end_.cur - 1)) / buffer_size + 1;
                    if (add_buffer > static_cast<size_type>((map_ + map_size_) - end_.node - 1))
                    {
                        reallocate_map(add_buffer, true);
                    }
                    else{
                        create_buffer(end_.node + 1, end_.node + add_buffer);
                    }

                }
                auto old_end = end_;
                auto new_end = end_ + n;
                const size_type elems_after = len - diff;
                pos = end_ - elems_after;
                if (elems_after > n)
                {
                    auto end_n = end_ - n;
                    mystl::uninitialized_copy(end_n, end_, end_);
                    end_ = new_end;
                    mystl::copy_back(pos, end_n, old_end);
                    mystl::fill(pos, pos + n, value_copy);
                }
                else
                {
                    mystl::uninitialized_fill(end_, pos + n, value_copy);
                    mystl::uninitialized_copy(pos, end_, pos + n);
                    end_ = new_end;
                    mystl::fill(pos, old_end, value_copy);
                }
            }
        }

    }
    template <class T>
    void deque<T>::assign(size_type n, const value_type& value){
        if(size() < n){
            for(auto cur_ = begin_; cur_ != end_; ++cur_){
                *cur_ = value;
            }
            insert(end_, n - size(), value);
        }
        else{
            erase(begin_ + n, end_);
            for(auto cur_ = begin_; cur_ != end_; ++cur_){
                *cur_ = value;
            }
        }

    }
    template <class T>
    typename deque<T>::iterator deque<T>::erase(iterator pos){
        auto next = pos;
        next++;
        const size_type n = pos - begin_;
        if(n < size() / 2){
            mystl::copy_back(begin_, pos, next);
            pop_front();
        }
        else{
            mystl::copy(next, end_, pos);
            pop_back();
        }
        return begin_ + n;
    }
    template <class T>
    typename deque<T>::iterator deque<T>::erase(iterator first, iterator last){
        const diff_type n = last - first;
        const size_type diff = first - begin_;
        if(diff < (size() - n) / 2){
            mystl::copy_back(first,begin_,last);
            iterator new_begin = begin_ + n;
            data_allocator::destroy(begin_.cur, new_begin.cur);
            begin_ = new_begin;
        }
        else{
            mystl::copy(last, end_, first);
            iterator new_end = end_ - n;
            data_allocator::destroy(new_end.cur, end_.cur);
            end_ = new_end;
        }
        return begin_ + n;
    }




}

#endif //MYTINYSTL_DEQUE_H
