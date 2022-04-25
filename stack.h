//
// Created by hbin on 2022/4/21.
//

#ifndef MYTINYSTL_STACK_H
#define MYTINYSTL_STACK_H
#include "deque.h"


namespace mystl{
    template <class T, class Container = mystl::deque<T>>
    class stack{
    public:
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;
        static_assert(std::is_same<T, value_type>::value, "the value_type of Container should be same with T");
    private:
        container_type ct_;
    public:
        stack() = default;
        explicit stack(size_type n) : ct_(n){}
        stack(size_type n, const value_type& value) : ct_(n, value){}

        template<class InputIter>
        stack(InputIter first, InputIter last) : ct_(first, last){}
        stack(std::initializer_list<T> ilist) : ct_(ilist.begin(), ilist.end()){}
        stack(const Container& c) : ct_(c){}
        stack(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value) : ct_(mystl::move(c)){}
        stack(const stack& rhs) : ct_(rhs.ct_){}
        stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value) : ct_(mystl::move(rhs.ct_)){}
        stack& operator=(const stack& rhs){
            ct_ = rhs.ct_;
            return *this;
        }
        stack& operator=(stack&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value){
            ct_ = mystl::move(rhs.ct_);
            return *this;
        }
        stack& operator=(std::initializer_list<T> ilist){
            ct_ = ilist;
            return *this;
        }
        ~stack() = default;

        reference top(){
            return ct_.back();
        }
        const_reference top() const{
            return ct_.back();
        }
        bool empty() noexcept {
            //return ct_.begin() == ct_.end();
            return ct_.empty();  //error??? why???
        }

        size_type size() const noexcept{
            return ct_.size();
        }
        template <class... Args>
        void emplace(Args&& ...args){
            ct_.emplace_back(mystl::forward<Args>(args)...);
        }
        void push(const value_type& value){
            ct_.push_back(value);
        }
        void push(value_type&& value){
            ct_.push_back(value);
        }
        value_type pop(){
            value_type temp = this->top();
            ct_.pop_back();
            return temp;
        }
        void clear(){
            while (!empty()){
                pop();
            }
        }
        void swap(stack& rhs) noexcept(noexcept(mystl::swap(ct_, rhs.ct_))){
            mystl::swap(ct_, rhs.ct_);
        }

    public:
        friend bool operator==(const stack& lhs, const stack& rhs) { return lhs.c_ == rhs.c_; }
        friend bool operator< (const stack& lhs, const stack& rhs) { return lhs.c_ <  rhs.c_; }


    };
    template <class T, class Container>
    bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
        return lhs == rhs;
    }
    template <class T, class Container>
    bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
        return lhs < rhs;
    }
    template <class T, class Container>
    bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
        return !(lhs == rhs);
    }
    template <class T, class Container>
    bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
        return rhs < lhs;
    }
    template <class T, class Container>
    bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
        return !(rhs < lhs);
    }
    template <class T, class Container>
    bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs){
        return !(lhs < rhs);
    }
    template <class T, class Container>
    void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))){
        lhs.swap(rhs);
    }

}



#endif //MYTINYSTL_STACK_H
