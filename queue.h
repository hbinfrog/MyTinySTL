//
// Created by hbin on 2022/4/21.
//

#ifndef MYTINYSTL_QUEUE_H
#define MYTINYSTL_QUEUE_H
#include "deque.h"
#include "vector.h"
#include "functional.h"
#include "algorithm.h"

namespace mystl{
    template<class T, class Container = mystl::deque<T>>
    class queue{
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;
        static_assert(std::is_same<T, value_type>::value, "the value_type of Container should be same with T");
    private:
        container_type ct_;
    public:
        queue() = default;
        explicit queue(size_type n) : ct_(n){}
        queue(size_type n, const value_type& value) : ct_(n, value){}
        template<class InputIter>
        queue(InputIter first, InputIter last) : ct_(first, last){}
        queue(std::initializer_list<T> ilist) : ct_(ilist.begin(), ilist.end()){}
        queue(const Container& c) : ct_(c){}
        queue(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value) : ct_(mystl::move(c)){}
        queue(const queue& rhs) : ct_(rhs.ct_){}
        queue(queue&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value) : ct_(mystl::move(rhs.ct_)){}
        queue& operator=(const queue& rhs){
            ct_ = rhs.ct_;
            return *this;
        }
        queue& operator=(queue&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value){
            ct_ = mystl::move(rhs.ct_);
            return *this;
        }
        queue& operator=(std::initializer_list<T> ilist){
            ct_ = ilist;
            return *this;
        }
        ~queue() = default;

        reference front(){
            return ct_.front();
        }
        const_reference front() const{
            return ct_.front();
        }
        reference back(){
            return ct_.back();
        }
        const_reference back() const{
            return ct_.back();
        }
        bool empty() noexcept {
            return ct_.empty();
        }
        size_type size() const noexcept{
            return ct_.size();
        }
        template <class ...Args>
        void emplace(Args&& ...args){
            ct_.emplace_back(mystl::forward<Args>(args)...);
        }
        void push(const value_type& value){
            ct_.push_back(value);
        }
        void push(value_type&& value){
            ct_.emplace_back(mystl::move(value));
        }
        void pop(){
            ct_.pop_front();
        }

        void clear(){
            while (!empty()){
                pop();
            }
        }
        void swap(queue& rhs) noexcept(noexcept(mystl::swap(ct_, rhs.ct_))){
            mystl::swap(ct_, rhs.ct_);
        }
    public:
        friend bool operator==(const queue& lhs, const queue& rhs) { return lhs.ct_ == rhs.ct_; }
        friend bool operator< (const queue& lhs, const queue& rhs) { return lhs.ct_ <  rhs.ct_; }
    };
    template <class T, class Container>
    bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs){
        return lhs == rhs;
    }
    template <class T, class Container>
    bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs){
        return !(lhs == rhs);
    }
    template <class T, class Container>
    bool operator<(const queue<T, Container>& lhs, const queue<T, Container>& rhs){
        return lhs < rhs;
    }
    template <class T, class Container>
    bool operator>(const queue<T, Container>& lhs, const queue<T, Container>& rhs){
        return rhs < lhs;
    }
    template <class T, class Container>
    bool operator<=(const queue<T, Container>& lhs, const queue<T, Container>& rhs){
        return !(rhs < lhs);
    }
    template <class T, class Container>
    bool operator>=(const queue<T, Container>& lhs, const queue<T, Container>& rhs){
        return !(lhs < rhs);
    }
    template <class T, class Container>
    void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))){
        lhs.swap(rhs);
    }

    template <class T, class Container = mystl::vector<T>, class Compare = mystl::less<typename Container::value_type>>
    class priority_queue{
    public:
        typedef Container                           container_type;
        typedef Compare                             value_compare;
        typedef typename Container::value_type      value_type;
        typedef typename Container::size_type       size_type;
        typedef typename Container::reference       reference;
        typedef typename Container::const_reference const_reference;
        static_assert(std::is_same<T, value_type>::value, "the value_type of Container should be same with T");

    private:
        container_type c_;     // 用底层容器来表现 priority_queue
        value_compare  comp_;  // 权值比较的标准
    public:
        priority_queue() = default;
        priority_queue(const Compare& c) : c_(), comp_(c){}
        explicit priority_queue(size_type n) : c_(n){
            mystl::make_heap(c_.begin(), c_.end(), comp_);
        }
        priority_queue(size_type n, const value_type& value) : c_(n, value){
            mystl::make_heap(c_.begin(), c_.end(), comp_);
        }
        template <class InputIter>
        priority_queue(InputIter first, InputIter last) :c_(first, last){
            mystl::make_heap(c_.begin(), c_.end(), comp_);
        }
        priority_queue(std::initializer_list<T> ilist) : c_(ilist){
            mystl::make_heap(c_.begin(), c_.end(), comp_);
        }
        priority_queue(const Container& c) : c_(c){
            mystl::make_heap(c_.begin(), c_.end(), comp_);
        }
        priority_queue(Container&& c) : c_(mystl::move(c)){
            mystl::make_heap(c_.begin(), c_.end(), comp_);
        }
        priority_queue(const priority_queue& rhs) : c_(rhs.c_), comp_(rhs.comp_){
            mystl::make_heap(c_.begin(), c_.end(), comp_);
        }
        priority_queue(priority_queue&& rhs) : c_(mystl::move(rhs.c_)), comp_(rhs.comp_){
            mystl::make_heap(c_.begin(), c_.end(), comp_);
        }
        priority_queue& operator=(const priority_queue& rhs){
            c_ = rhs.c_;
            comp_ = rhs.comp_;
            mystl::make_heap(c_.begin(), c_.end(), comp_);
            return *this;
        }
        priority_queue& operator=(priority_queue&& rhs){
            c_ = mystl::move(rhs.c_);
            comp_ = rhs.comp_;
            mystl::make_heap(c_.begin(), c_.end(), comp_);
            return *this;
        }
        priority_queue& operator=(std::initializer_list<T> ilist){
            c_ = ilist;
            comp_ = value_compare();
            mystl::make_heap(c_.begin(), c_.end(), comp_);
            return *this;
        }
        ~priority_queue() = default;

    public:

        const_reference top() const{
            return c_.front();
        }
        bool empty() noexcept{
            return c_.empty();
        }
        size_type size() const noexcept{
            return c_.size();
        }
        template <class... Args>
        void emplace(Args&& ...args){
            c_.emplace_back(mystl::forward<Args>(args)...);
            mystl::push_heap(c_.begin(), c_.end(), comp_);
        }
        void push(const value_type& value){
            c_.push_back(value);
            mystl::push_heap(c_.begin(), c_.end(), comp_);
        }
        void push(value_type&& value){
            c_.push_back(mystl::move(value));
            mystl::push_heap(c_.begin(), c_.end(), comp_);
        }
        void pop(){
            mystl::pop_heap(c_.begin(), c_.end(), comp_);
            c_.pop_back();
        }
        void clear(){
            while (!empty()){
                pop();
            }
        }
        void swap(priority_queue& rhs) noexcept(noexcept(mystl::swap(c_, rhs.c_)) && noexcept(mystl::swap(comp_, rhs.comp_))){
            mystl::swap(c_, rhs.c_);
            mystl::swap(comp_, rhs.comp_);
        }

    public:
        friend bool operator==(const priority_queue& lhs, const priority_queue& rhs){
            return lhs.c_ == rhs.c_;
        }
        friend bool operator!=(const priority_queue& lhs, const priority_queue& rhs){
            return lhs.c_ != rhs.c_;
        }
    private:

    };


}

#endif //MYTINYSTL_QUEUE_H
