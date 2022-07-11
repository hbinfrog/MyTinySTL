//
// Created by hbin on 2022/5/7.
//

#ifndef MYTINYSTL_RB_TREE_H
#define MYTINYSTL_RB_TREE_H
#include <initializer_list>

#include <cassert>
#include "functional.h"
#include "iterator.h"
#include "memory.h"
#include "type_traits.h"
#include "exceptdef.h"

namespace mystl{
    static constexpr bool rb_tree_red = false;
    static constexpr bool rb_tree_black = true;
    //algorithm
    template <class NodePtr>
    NodePtr rb_tree_min(NodePtr x) noexcept
    {
        while(x->left) x = x->left;
        return x;
    }
    template <class NodePtr>
    NodePtr rb_tree_max(NodePtr x) noexcept
    {
        while (x->right) x = x->right;
        return x;
    }
    template <class NodePtr>
    bool node_is_red(NodePtr node) noexcept
    {
        return node->color == rb_tree_red;
    }
    template <class NodePtr>
    bool node_is_black(NodePtr node) noexcept
    {
        return node->color == rb_tree_black;
    }
    template <class NodePtr>
    void node_set_red(NodePtr& node) noexcept
    {
        node->color = rb_tree_red;
    }
    template <class NodePtr>
    void node_set_black(NodePtr& node) noexcept
    {
        node->color = rb_tree_black;
    }
    template <class NodePtr>
    NodePtr rb_tree_next(NodePtr node) noexcept
    {
        if(node->right)
            return rb_tree_min(node->right);
        while(!(node == node->parent->left))
            node = node->parent;
        return node->parent;
    }

    /*---------------------------------------*\
    |       p                         p       |
    |      / \                       / \      |
    |     x   d    rotate left      y   d     |
    |    / \       ===========>    / \        |
    |   a   y                     x   c       |
    |      / \                   / \ / \      |
    |     b   c                 a   b         |
    |        / \                              |
    \*---------------------------------------*/
    //这里假设的是c存在子节点使得平衡树失衡，如果是b可以先进行一次右旋，再进行左旋
    template <class NodePtr>
    void rb_tree_rotate_left(NodePtr x, NodePtr& root) noexcept
    {
        auto y = x->right;
        x->right = y->left;
        if(y->left)
            y->left->parent = x;
        y->parent = x->parent;
        if(x == root)
            root = y;
        else if(x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }
    /*----------------------------------------*\
    |     p                         p          |
    |    / \                       / \         |
    |   d   x      rotate right   d   y        |
    |      / \     ===========>      / \       |
    |     y   a                     b   x      |
    |    / \                       / \ / \     |
    |   b   c                         c   a    |
    |  / \
    \*----------------------------------------*/
    //这里假设的是b存在子节点使得平衡树失衡，如果是c可以先进行一次左旋，再进行右旋
    template <class NodePtr>
    void rb_tree_rotate_right(NodePtr x, NodePtr& root) noexcept
    {
        auto y = x->left;
        x->left = y->right;
        if(y->right)
            y->right->parent = x;
        y->parent = x->parent;
        if(x == root) root = y;
        else if(x == x->parent->left) x->parent->left = y;
        else x->parent->right = y;
        y->right = x;
        x->parent = y;
    }

    //https://www.bilibili.com/video/BV1Ar4y1Q7Js?spm_id_from=333.999.0.0
    template <class NodePtr>
    void rb_tree_insert_rebalance(NodePtr x, NodePtr& root) noexcept
    {
        node_set_red(x);
        while(x != root && node_is_red(x->parent))
        {
            if(x == x->parent->left)
            {
                auto uncle = x->parent->parent->right;
                if(uncle && node_is_red(uncle))
                {
                    node_set_black(x->parent);
                    node_set_black(uncle);
                    x = x->parent->parent;
                    node_set_red(x);
                }
                else
                {
                    if(x == x->parent->right)
                    {
                        x = x->parent;
                        rb_tree_rotate_left(x, root);
                    }
                    node_set_black(x->parent);
                    node_set_red(x->parent->parent);
                    rb_tree_rotate_right(x->parent->parent, root);
                    break;
                }
            }
            else
            {
                auto uncle = x->parent->parent->left;
                if(uncle && node_is_red(uncle))
                {
                    node_set_black(x->parent);
                    node_set_black(uncle);
                    x = x->parent->parent;
                    node_set_red(x);
                }
                else
                {
                    if(x == x->parent->left)
                    {
                        x = x->parent;
                        rb_tree_rotate_right(x, root);
                    }
                    node_set_black(x->parent);
                    node_set_red(x->parent->parent);
                    rb_tree_rotate_left(x->parent->parent, root);
                    break;
                }
            }
        }
        node_set_black(root);
    }
    template <class NodePtr>
    NodePtr rb_tree_erase_rebalance(NodePtr z, NodePtr& root, NodePtr& leftmost, NodePtr& rightmost)
    {

    }



    template <class T, class Compare>
    class rb_tree{





    };
}

#endif //MYTINYSTL_RB_TREE_H
