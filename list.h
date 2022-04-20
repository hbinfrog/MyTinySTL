//
// Created by hbin on 2022/4/18.
//

#ifndef MYTINYSTL_LIST_H
#define MYTINYSTL_LIST_H
#include <initializer_list>

#include "iterator.h"
#include "memory.h"
#include "functional.h"
#include "util.h"
#include "exceptdef.h"
#include <list>

namespace mystl{
    template <class T>
    struct list_node{
        list_node<T>* pred;
        list_node<T>* succ;
        T val;
        list_node() = default;
        list_node(const T& value, list_node<T>* p = nullptr, list_node<T>* s = nullptr) : val(value), pred(p), succ(s){}
    };
    template <class T>
    struct list_iterator : public mystl::iterator<mystl::bidirectional_iterator_tag, T>{
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef list_node<T>* node_ptr;
        typedef list_iterator<T> self;
        node_ptr node_;
        list_iterator() = default;
        list_iterator(node_ptr x) : node_(x){}
        list_iterator(const self& x) : node_(x.node_){}

        reference operator*()const{
            return node_->val;
        }
        pointer operator->() const{
            return &(operator*());
        }
        self& operator++(){
            MYSTL_DEBUG(node_ != nullptr);
            node_ = node_->succ;
            return *this;
        }
        self operator++(int){
            self temp = *this;
            ++*this;
            return temp;
        }
        self& operator--(){
            MYSTL_DEBUG(node_ != nullptr);
            node_ = node_->pred;
            return *this;
        }
        self operator--(int){
            self temp = *this;
            --*this;
            return temp;
        }
        bool operator==(const self& rhs){
            return node_ == rhs.node_;
        }
        bool operator!=(const self& rhs){
            return node_ != rhs.node_;
        }
    };
    template <class T>
    struct list_const_iterator : public iterator<bidirectional_iterator_tag, T>{
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef list_node<T>* node_ptr;
        typedef list_const_iterator<T> self;
        node_ptr node_;
        list_const_iterator() = default;
        list_const_iterator(node_ptr x) : node_(x){}
        list_const_iterator(const list_const_iterator& x) : node_(x.node_){}
        list_const_iterator(const list_iterator<T>& x) : node_(x.node_){}
        reference operator*()const{
            return node_->val;
        }
        pointer operator->() const{
            return &(operator*());
        }
        self& operator++(){
            MYSTL_DEBUG(node_ != nullptr);
            node_ = node_->succ;
            return *this;
        }
        self operator++(int){
            self temp = *this;
            ++*this;
            return temp;
        }
        self& operator--(){
            MYSTL_DEBUG(node_ != nullptr);
            node_ = node_->pred;
            return *this;
        }
        self operator--(int){
            self temp = *this;
            --*this;
            return temp;
        }
        bool operator==(const self& rhs){
            return node_ == rhs.node_;
        }
        bool operator!=(const self& rhs){
            return node_ != rhs.node_;
        }
    };
    template <class T>
    class list{
    public:
        typedef mystl::allocator<T>                      allocator_type;
        typedef mystl::allocator<T>                      data_allocator;
        //typedef mystl::allocator<list_node_base<T>>      base_allocator;
        typedef mystl::allocator<list_node<T>>           node_allocator;

        typedef typename allocator_type::value_type      value_type;
        typedef typename allocator_type::pointer         pointer;
        typedef typename allocator_type::const_pointer   const_pointer;
        typedef typename allocator_type::reference       reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::size_type       size_type;
        typedef typename allocator_type::diff_type diff_type;

        typedef list_iterator<T>                         iterator;
        typedef list_const_iterator<T>                   const_iterator;
        typedef mystl::reverse_iterator<iterator>        reverse_iterator;
        typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;
        typedef list_node<T>* node_ptr;


    private:
        size_type size_;
        node_ptr trailer_;
        node_ptr header_;
    public:
        list(){
            init();
        }
        explicit list(size_type n){
            fill_init(n, value_type());
        }
        list(size_type n, const T& value){
            fill_init(n,value);
        }
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        list(Iter first, Iter last){
            copy_init(first, last);
        }
        list(std::initializer_list<T> ilist){
            copy_init(ilist.begin(), ilist.end());
        }
        list(const list& rhs){
            copy_init(rhs.cbegin(), rhs.cend());
        }
        list& operator=(const list& rhs);
        list& operator=(list&& rhs) noexcept;
        list& operator=(std::initializer_list<T> ilist);
        ~list(){
            if(header_ != nullptr && trailer_ != nullptr){
                clear();
                node_allocator::deallocate(header_);
                node_allocator::deallocate(trailer_);
                header_ = nullptr;
                trailer_ = nullptr;
                size_ = 0;
            }
        }
        iterator begin() noexcept{
            return header_->succ;
        }
        const_iterator begin() const noexcept{
            return header_->succ;
        }
        iterator end() noexcept{
            return trailer_;
        }
        const_iterator end() const noexcept{
            return trailer_;
        }
        reverse_iterator rbegin() noexcept{
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin()  const noexcept{
            return reverse_iterator(end());
        }
        reverse_iterator       rend()          noexcept{
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend()    const noexcept{
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
        const_reverse_iterator crend()   const noexcept{
            return rend();
        }
        bool empty() const noexcept{
            return header_->succ == trailer_;
        }
        size_type size() const noexcept{
            return size_;
        }
        size_type maxsize() const noexcept{
            return static_cast<size_type>(-1);
        }
        reference front(){
            MYSTL_DEBUG(!empty());
            return *begin();
        }
        const_reference front() const{
            MYSTL_DEBUG(!empty());
            return *begin();
        }
        reference back(){
            MYSTL_DEBUG(!empty());
            return *(--end());
        }
        const_reference back() const{
            MYSTL_DEBUG(!empty());
            return *(--end());
        }
        void assign(size_type n, const value_type& value){
            fill_assign(n,value);
        }
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        void assign(Iter first, Iter last){
            copy_assign(first, last);
        }
        void assign(std::initializer_list<T> ilist){
            copy_assign(ilist.begin(), ilist.end());
        }
        template <class ...Args>
        void emplace_front(Args&& ...args);
        template <class ...Args>
        void emplace_back(Args&& ...args);
        template <class ...Args>
        iterator emplace(const_iterator pos, Args&& ...args);
        iterator insert(const_iterator pos, const value_type& value);
        iterator insert(const_iterator pos, value_type&& value);
        iterator insert(const_iterator pos, size_type n, const value_type& value);
        template <class Iter, typename std::enable_if<mystl::is_input_iterator<Iter>::value, int>::type = 0>
        iterator insert(const_iterator pos, Iter first, Iter last){
            size_type n = mystl::distance(first, last);
            THROW_LENGTH_ERROR_IF(size_ > maxsize() - n, "list<T>'s size too big");
            for(; n > 0; n--){
                --last;
                insert(pos, *last);
            }
        }
        void push_front(const value_type& value);
        void push_front(value_type&& value);
        void push_back(const value_type& value);
        void push_back(value_type&& value);
        void pop_front();
        void pop_back();
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        void clear();
        void resize(size_type new_size);
        void resize(size_type new_size, const value_type& value);
        void swap(list& rhs) noexcept;
        void splice(const_iterator pos, list& other);
        void splice(const_iterator pos, list& other, const_iterator it);
        void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);
        void remove(const value_type& value);
        template <class UnaryPredicate>
        void remove_if(UnaryPredicate pre);
        void unique();
        template <class BinaryPredicate>
        void unique(BinaryPredicate pre);
        void merge(list& x);
        template <class Compare>
        void merge(list& x, Compare comp);
        void sort();
        template <class Compared>
        void sort(Compared comp);


        void reverse();



    private:
        void init();
        void fill_init(size_type n, const value_type& val);
        template <class Iter>
        void copy_init(Iter first, Iter last);
        template <class ...Args>
        node_ptr create_node(Args&& ...args);
        void destroy_node(node_ptr p);
        void insertAsLast(node_ptr p);
        void insertAsFirst(node_ptr p);
        void unlink_nodes(node_ptr first, node_ptr last);
        void link_node_back(node_ptr pos, node_ptr n);
        void link_node_front(node_ptr n, node_ptr pos);
        void link_nodes(node_ptr first, node_ptr last, node_ptr pos);
        void fill_assign(size_type n, const value_type& value);

        template<class Iter>
        void copy_assign(Iter first, Iter last);
        template <class Compared>
        iterator list_sort(iterator f1, iterator l2, size_type n, Compared comp);




    };
    template <class T>
    void list<T>::init(){
        size_ = 0;
        header_ = node_allocator::allocate(1);
        trailer_ = node_allocator::allocate(1);
        try{
            data_allocator::construct(mystl::address_of(header_->val), mystl::forward<T>(value_type()));
            header_->succ = trailer_;
            header_->pred = nullptr;
        }
        catch (...){
            node_allocator::deallocate(header_);
            throw;
        }
        try{
            data_allocator::construct(mystl::address_of(trailer_->val), value_type());
            trailer_->pred = header_;
            trailer_->succ = nullptr;
        }
        catch (...){
            node_allocator::deallocate(trailer_);
            throw;
        }

    }
    template <class T>
    void list<T>::fill_init(size_type n, const value_type& val){
        init();
        try{
            for(; n > 0; n--){
                node_ptr node = create_node(val);
                insertAsLast(node);
            }
        }
        catch(...){
            clear();
            node_allocator::deallocate(header_);
            node_allocator::deallocate(trailer_);
            header_ = trailer_ = nullptr;
            throw;
        }

    }
    template <class T>
    template <class ...Args>
    typename list<T>::node_ptr
    list<T>::create_node(Args&& ...args){
        node_ptr p = node_allocator::allocate(1);
        try
        {
            data_allocator::construct(mystl::address_of(p->val), mystl::forward<Args>(args)...);
            p->pred = nullptr;
            p->succ = nullptr;
        }
        catch (...)
        {
            node_allocator::deallocate(p);
            throw;
        }
        return p;
    }
    template <class T>
    void list<T>::insertAsLast(node_ptr p){
        size_++;
        p->pred = trailer_->pred;
        p->succ = trailer_;
        trailer_->pred->succ = p;
        trailer_->pred = p;
    }
    template <class T>
    void list<T>::insertAsFirst(node_ptr p){
        size_++;
        p->pred = header_;
        p->succ = header_->succ;
        header_->succ->pred = p;
        header_->succ = p;
    }
    template <class T>
    template <class Iter>
    void list<T>::copy_init(Iter first, Iter last){
        init();
        size_type n = mystl::distance(first, last);
        try{
            for(; n > 0; --n, ++first){
                //const value_type& val = *first;
                node_ptr node = create_node(*first);
                insertAsLast(node);
            }
        }
        catch (...){
            clear();
            node_allocator::deallocate(header_);
            node_allocator::deallocate(trailer_);
            header_ = trailer_ = nullptr;
            throw;
        }

    }
    template <class T>
    void list<T>::destroy_node(node_ptr p){
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        data_allocator::destroy(mystl::address_of(p->val));
        node_allocator::deallocate(p);
        size_--;
    }
    template <class T>
    void list<T>::clear(){
        while (0 < size_){
            destroy_node(header_->succ);
        }
    }
    template <class T>
    list<T>& list<T>::operator=(const list& rhs){
        if(this != &rhs){
            assign(rhs.begin(), rhs.end());
        }
        return *this;
    }//error
    template <class T>
    list<T>& list<T>::operator=(list&& rhs) noexcept{
        clear();
        size_type n = rhs.size();
        node_ptr cur = rhs.header_->succ;
        while(n--){
            //const value_type& val = cur->val;
            node_ptr node = create_node(cur->val);
            insertAsLast(node);
            cur = cur->succ;
        }
        rhs.clear();
        return *this;
    }
    template <class T>
    list<T>& list<T>::operator=(std::initializer_list<T> ilist){
        list<T> temp(ilist.begin(), ilist.end());
        swap(temp);
        return *this;

    }
    template <class T>
    void list<T>::swap(list& rhs) noexcept{
        mystl::swap(header_, rhs.header_);
        mystl::swap(trailer_, rhs.trailer_);
        mystl::swap(size_, rhs.size_);
    }
    template <class T>
    typename list<T>::iterator list<T>::erase(const_iterator pos){
        MYSTL_DEBUG(pos != cend());
        node_ptr p = pos.node_;
        node_ptr succ = p->succ;
        destroy_node(p);
        return iterator(succ);
    }
    template <class T>
    typename list<T>::iterator list<T>::erase(const_iterator first, const_iterator last){
        while (first != last){
            first = erase(first);
        }
        return iterator(last.node_);
    }
    template <class T>
    typename list<T>::iterator list<T>::insert(const_iterator pos, const value_type& value){
        THROW_LENGTH_ERROR_IF(size_ > maxsize() - 1, "list<T>'s size too big");
        node_ptr p = create_node(value);
        link_node_back(pos.node_, p);
        size_++;
        return iterator(p);
    }

    template <class T>
    void list<T>::link_node_back(node_ptr pos, node_ptr n){
        n->pred = pos;
        n->succ = pos->succ;
        pos->succ->pred = n;
        pos->succ = n;
    }
    template <class T>
    void list<T>::link_node_front(node_ptr n, node_ptr pos){
        n->succ = pos;
        n->pred = pos->pred;
        pos->pred->succ = n;
        pos->pred = n;
    }
    template <class T>
    typename list<T>::iterator list<T>::insert(const_iterator pos, value_type&& value){
        THROW_LENGTH_ERROR_IF(size_ > maxsize() - 1, "list<T>'s size too big");
        node_ptr p = create_node(mystl::move(value));
        link_node_back(pos.node_, p);
        size_++;
        return iterator(p);
    }
    template <class T>
    typename list<T>::iterator list<T>::insert(const_iterator pos, size_type n, const value_type& value){
        THROW_LENGTH_ERROR_IF(size_ > maxsize() - n, "list<T>'s size too big");
        for(; n > 0; n--){
            pos = insert(pos,value);
        }

    }
    template <class T>
    void list<T>::fill_assign(size_type n, const value_type& value){
        iterator first = begin();
        iterator last = end();
        for(; n > 0 && first != last; --n, ++first){
            *first = value;
        }
        if(n > 0){
            insert(last, n, value);
        }
        else{
            erase(first,last);
        }
    }
    template <class T>
    template <class Iter>
    void list<T>::copy_assign(Iter first, Iter last){
        iterator first1 = begin();
        iterator last1 = end();
        for(; first != last && first1 != last1; ++first, ++first1){
            *first = *first1;
        }
        if(first == last){
            erase(first1,last1);
        }
        else{
            insert(first1, first, last);
        }
    }
    template <class T>
    template <class ...Args>
    void list<T>::emplace_front(Args&& ...args){
        THROW_LENGTH_ERROR_IF(size_ > maxsize() - 1, "list<T>'s size too big");
        auto link_node = create_node(mystl::forward<Args>(args)...);
        insertAsFirst(link_node);
    }
    template <class T>
    template <class ...Args>
    void list<T>::emplace_back(Args&& ...args){
        THROW_LENGTH_ERROR_IF(size_ > maxsize() - 1, "list<T>'s size too big");
        auto link_node = create_node(mystl::forward<Args>(args)...);
        insertAsLast(link_node);
    }
    template <class T>
    template <class ...Args>
    typename list<T>::iterator list<T>::emplace(const_iterator pos, Args&& ...args){
        THROW_LENGTH_ERROR_IF(size_ > maxsize() - 1, "list<T>'s size too big");
        auto link_node = create_node(mystl::forward<Args>(args)...);
        link_node_front(link_node, pos.node_);
        size_++;
        return iterator(link_node);
    }
    template <class T>
    void list<T>::push_front(const value_type& value){
        THROW_LENGTH_ERROR_IF(size_ > maxsize() - 1, "list<T>'s size too big");
        auto link_node = create_node(value);
        insertAsFirst(link_node);
    }
    template <class T>
    void list<T>::push_front(value_type&& value){
        emplace_front(mystl::move(value));
    }
    template <class T>
    void list<T>::push_back(const value_type& value){
        THROW_LENGTH_ERROR_IF(size_ > maxsize() - 1, "list<T>'s size too big");
        auto link_node = create_node(value);
        insertAsLast(link_node);
    }
    template <class T>
    void list<T>::push_back(value_type&& value){
        emplace_back(mystl::move(value));
    }
    template <class T>
    void list<T>::pop_front(){
        MYSTL_DEBUG(!empty());
        destroy_node(header_->succ);
    }
    template <class T>
    void list<T>::pop_back() {
        MYSTL_DEBUG(!empty());
        destroy_node(trailer_->pred);
    }
    template <class T>
    void list<T>::resize(size_type new_size){
        resize(new_size, value_type());
    }
    template <class T>
    void list<T>::resize(size_type new_size, const value_type& value){
        auto cur = begin();
        size_type len = 0;
        for(;cur != end() && len < new_size;++cur, ++len);
        if(len == new_size){
            erase(cur, end());
        }
        else{
            insert(end(), new_size - len, value);
        }
    }
    //splice front of the pos
    template <class T>
    void list<T>::splice(const_iterator pos, list& other){
        MYSTL_DEBUG(this != &other);
        if(!other.empty()){
            THROW_LENGTH_ERROR_IF(size_ > maxsize() - other.size_, "list<T>'s size too big");
            auto first = other.header_->succ;
            auto last = other.trailer_->pred;
            other.unlink_nodes(first, last);
            link_nodes(first,last,pos.node_);
            size_ += other.size_;
            other.size_ = 0;
        }
    }
    template <class T>
    void list<T>::splice(const_iterator pos, list& other, const_iterator it){
        if (pos.node_ != it.node_ && pos.node_ != it.node_->next){
            THROW_LENGTH_ERROR_IF(size_ > maxsize() - 1, "list<T>'s size too big");
            auto node = it.node_;
            other.unlink_nodes(node,node);
            link_nodes(node,node,pos.node_);
            size_++;
            other.size_--;
        }
    }
    template <class T>
    void list<T>::splice(const_iterator pos, list& other, const_iterator first, const_iterator last){
        if(first != last && this != &other){
            size_type n = mystl::distance(first,last);
            THROW_LENGTH_ERROR_IF(size_ > maxsize() - n, "list<T>'s size too big");
            auto first_node = first.node_;
            auto last_node = last.node_->pred;
            other.unlink_nodes(first_node, last_node);
            link_nodes(first_node,last_node,pos.node_);
            size_ += n;
            other.size_ -= n;

        }
    }
    template <class T>
    void list<T>::unlink_nodes(node_ptr first, node_ptr last){
        first->pred->succ = last->succ;
        last->succ->pred = first->pred;
    }
    template <class T>
    void list<T>::link_nodes(node_ptr first, node_ptr last, node_ptr pos){
        pos->pred->succ = first;
        first->pred = pos->pred;
        pos->pred = last;
        last->succ = pos;
    }
    template <class T>
    void list<T>::reverse(){
        if(size_ < 2){
            return;
        }
        for(auto p = header_; p; p = p->pred){
            mystl::swap(p->pred, p->succ);
        }
        mystl::swap(header_, trailer_);

    }
    template <class T>
    template <class UnaryPredicate>
    void list<T>::remove_if(UnaryPredicate pre){
        auto cur = begin();
        for (auto next = cur; cur != end(); cur = next)
        {
            ++next;
            if (pre(*cur))
            {
                erase(cur);
            }
        }
    }
    template <class T>
    void list<T>::remove(const value_type& value){
        remove_if([&](const value_type& val){return val == value;});
    }
    template <class T>
    template <class BinaryPredicate>
    void list<T>::unique(BinaryPredicate pre){
        auto first1 = begin();
        auto last = end();
        auto first2 = first1;
        ++first2;
        while (first2 != last){
            if(pre(*first1, *first2)){
                erase(first2);
            }
            else{
                first1 = first2;
            }
            first2 = first1;
            ++first2;
        }


    }
    template <class T>
    void list<T>::unique(){
        unique(mystl::equal_to<T>());
    }
    template <class T>
    template <class Compare>
    void list<T>::merge(list& x, Compare comp){
        if(this != &x){
            THROW_LENGTH_ERROR_IF(size_ > maxsize() - x.size_, "list<T>'s size too big");
            auto f1 = begin();
            auto l1 = end();
            auto f2 = x.begin();
            auto l2 = x.end();

            while (f1 != l1 && f2 != l2)
            {
                if (comp(*f2, *f1))
                {
                    auto next = f2;
                    ++next;
                    while(next != l2 && comp(*next, *f1)){
                        ++next;
                    }
                    auto f = f2.node_;
                    auto l = next.node_->pred;
                    f2 = next;
                    x.unlink_nodes(f, l);
                    link_nodes(f, l, f1.node_);
                    ++f1;
                }
                else
                {
                    ++f1;
                }
            }
            if (f2 != l2)
            {
                auto f = f2.node_;
                auto l = l2.node_->pred;
                x.unlink_nodes(f, l);
                link_nodes(f, l,l1.node_);
            }
            size_ += x.size_;
            x.size_ = 0;
        }
    }
    template <class T>
    void list<T>::merge(list& x){
        merge(x, mystl::less<T>());
    }
    template <class T>
    template <class Compared>
    void list<T>::sort(Compared comp){
        list_sort(begin(),end(),size(),comp());
    }
    template <class T>
    void list<T>::sort(){
        list_sort(begin(), end(), size(), mystl::less<T>());
    }
    template <class T>
    template <class Compared>
    typename list<T>::iterator list<T>::
            list_sort(iterator f1, iterator l2, size_type n, Compared comp){
        if(n < 2){
            return f1;
        }
        size_type mid = n / 2;
        auto l1 = f1;
        mystl::advance(l1,mid);
        list_sort(f1, l1, mid,comp());
        list_sort(l1, l2, n - mid, comp());
        //merge()
    }

    template <class T>
    bool operator==(const list<T>& lhs, const list<T>& rhs){
        auto fl = lhs.cbegin();
        auto ll = lhs.cend();
        auto fr = rhs.cbegin();
        auto lr = rhs.cend();
        while (fl != ll && fr != lr && *fl == *fr){
            ++fl;
            ++fr;
        }
        return fl == ll && fr == lr;
    }
    template <class T>
    bool operator<(const list<T>& lhs, const list<T>& rhs){
        return mystl::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
    }
    template <class T>
    bool operator!=(const list<T>& lhs, const list<T>& rhs){
        return !(lhs == rhs);
    }
    template <class T>
    bool operator>(const list<T>& lhs, const list<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    bool operator<=(const list<T>& lhs, const list<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    bool operator>=(const list<T>& lhs, const list<T>& rhs)
    {
        return !(lhs < rhs);
    }
    template <class T>
    void swap(list<T>& lhs, list<T>& rhs) noexcept
    {
        lhs.swap(rhs);
    }



}
#endif //MYTINYSTL_LIST_H

