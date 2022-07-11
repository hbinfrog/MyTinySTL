//
// Created by hbin on 2022/4/27.
//

#ifndef MYTINYSTL_MAP_H
#define MYTINYSTL_MAP_H
#include "rb_tree.h"
namespace mystl{
    template <class Key, class T, class Compare = mystl::less<Key>>
    class map{
        typedef Key                        key_type;
        typedef T                          mapped_type;
        typedef mystl::pair<const Key, T>  value_type;
        typedef Compare                    key_compare;

    private:
        typedef mystl::rb_tree<value_type, key_compare>  base_type;
        base_type tree_;
    };

}


#endif //MYTINYSTL_MAP_H
