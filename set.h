//
// Created by hbin on 2022/4/27.
//

#ifndef MYTINYSTL_SET_H
#define MYTINYSTL_SET_H
#include "rb_tree.h"
namespace mystl{
    template <class Key, class Compare = mystl::less<Key>>
    class set{
    public:
        typedef Key        key_type;
        typedef Key        value_type;
        typedef Compare    key_compare;
        typedef Compare    value_compare;


    private:
        typedef mystl::rb_tree<value_type, key_compare> base_type;
        base_type tree_;

    };
}
#endif //MYTINYSTL_SET_H

