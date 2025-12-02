#ifndef TTKTREE_H
#define TTKTREE_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "ttkmoduleexport.h"

/*! @brief The class of the tree modules.
 * @author Greedysky <greedysky@163.com>
 */
//Red-Black Tree
template <typename _Key, typename _Value>
struct TTK_MODULE_EXPORT TTKRedBlackTree
{
public:
    using key_type = _Key;
    using value_type = _Value;
    using reference = _Value&;
    using const_reference = const _Value&;

private:
    struct Node
    {
        key_type m_key;
        value_type m_value;
        Node* m_left;
        Node* m_right;
        Node* m_parent;
        alignas(sizeof(void*)) bool m_b;

        Node(const key_type &k, const value_type &v) noexcept
            : m_key(k),
              m_value(v),
              m_left(nullptr),
              m_right(nullptr),
              m_parent(nullptr),
              m_b(false)
        {

        }

        Node(const key_type &k, value_type &&v) noexcept
            : m_key(k),
              m_value(std::move(v)),
              m_left(nullptr),
              m_right(nullptr),
              m_parent(nullptr),
              m_b(false)
        {

        }
    };

    Node* m_root;
    Node* m_nil;

    inline void rotateLeft(Node *x) noexcept
    {
        Node *y = x->m_right;
        x->m_right = y->m_left;

        if(y->m_left != m_nil)
        {
            y->m_left->m_parent = x;
        }
        y->m_parent = x->m_parent;

        if(x->m_parent == m_nil)
        {
            m_root = y;
        }
        else if(x == x->m_parent->m_left)
        {
            x->m_parent->m_left = y;
        }
        else
        {
            x->m_parent->m_right = y;
        }

        y->m_left = x;
        x->m_parent = y;
    }

    inline void rotateRight(Node *x) noexcept
    {
        Node *y = x->m_left;
        x->m_left = y->m_right;

        if(y->m_right != m_nil)
        {
            y->m_right->m_parent = x;
        }
        y->m_parent = x->m_parent;

        if(x->m_parent == m_nil)
        {
            m_root = y;
        }
        else if(x == x->m_parent->m_right)
        {
            x->m_parent->m_right = y;
        }
        else
        {
            x->m_parent->m_left = y;
        }

        y->m_right = x;
        x->m_parent = y;
    }

    void fixInsert(Node *z) noexcept
    {
        while(z != m_root && !z->m_parent->m_b)
        {
            if(z->m_parent == z->m_parent->m_parent->m_left)
            {
                Node *y = z->m_parent->m_parent->m_right;
                if(!y->m_b)
                {
                    z->m_parent->m_b = true;
                    y->m_b = true;
                    z->m_parent->m_parent->m_b = false;
                    z = z->m_parent->m_parent;
                }
                else
                {
                    if(z == z->m_parent->m_right)
                    {
                        z = z->m_parent;
                        rotateLeft(z);
                    }

                    z->m_parent->m_b = true;
                    z->m_parent->m_parent->m_b = false;
                    rotateRight(z->m_parent->m_parent);
                }
            }
            else
            {
                Node *y = z->m_parent->m_parent->m_left;
                if(!y->m_b)
                {
                    z->m_parent->m_b = true;
                    y->m_b = true;
                    z->m_parent->m_parent->m_b = false;
                    z = z->m_parent->m_parent;
                }
                else
                {
                    if(z == z->m_parent->m_left)
                    {
                        z = z->m_parent;
                        rotateRight(z);
                    }

                    z->m_parent->m_b = true;
                    z->m_parent->m_parent->m_b = false;
                    rotateLeft(z->m_parent->m_parent);
                }
            }
        }

        m_root->m_b = true;
    }

    inline void transplant(Node *u, Node *v) noexcept
    {
        if(u->m_parent == m_nil)
        {
            m_root = v;
        }
        else if(u == u->m_parent->m_left)
        {
            u->m_parent->m_left = v;
        }
        else
        {
            u->m_parent->m_right = v;
        }
        v->m_parent = u->m_parent;
    }

    void fixDelete(Node *x) noexcept
    {
        while(x != m_root && x->m_b)
        {
            if(x == x->m_parent->m_left)
            {
                Node *w = x->m_parent->m_right;
                if(!w->m_b)
                {
                    w->m_b = true;
                    x->m_parent->m_b = false;
                    rotateLeft(x->m_parent);
                    w = x->m_parent->m_right;
                }

                if(w->m_left->m_b && w->m_right->m_b)
                {
                    w->m_b = false;
                    x = x->m_parent;
                }
                else
                {
                    if(w->m_right->m_b)
                    {
                        w->m_left->m_b = true;
                        w->m_b = false;
                        rotateRight(w);
                        w = x->m_parent->m_right;
                    }

                    w->m_b = x->m_parent->m_b;
                    x->m_parent->m_b = true;
                    w->m_right->m_b = true;
                    rotateLeft(x->m_parent);
                    x = m_root;
                }
            }
            else
            {
                Node *w = x->m_parent->m_left;
                if(!w->m_b)
                {
                    w->m_b = true;
                    x->m_parent->m_b = false;
                    rotateRight(x->m_parent);
                    w = x->m_parent->m_left;
                }

                if(w->m_right->m_b && w->m_left->m_b)
                {
                    w->m_b = false;
                    x = x->m_parent;
                }
                else
                {
                    if(w->m_left->m_b)
                    {
                        w->m_right->m_b = true;
                        w->m_b = false;
                        rotateLeft(w);
                        w = x->m_parent->m_left;
                    }

                    w->m_b = x->m_parent->m_b;
                    x->m_parent->m_b = true;
                    w->m_left->m_b = true;
                    rotateRight(x->m_parent);
                    x = m_root;
                }
            }
        }

        x->m_b = true;
    }

    void deleteNode(Node *z) noexcept
    {
        Node *y = z;
        bool yColor = y->m_b;
        Node *x;

        if(z->m_left == m_nil)
        {
            x = z->m_right;
            transplant(z, z->m_right);
        }
        else if(z->m_right == m_nil)
        {
            x = z->m_left;
            transplant(z, z->m_left);
        }
        else
        {
            y = minimum(z->m_right);
            yColor = y->m_b;
            x = y->m_right;

            if(y->m_parent == z)
            {
                x->m_parent = y;
            }
            else
            {
                transplant(y, y->m_right);
                y->m_right = z->m_right;
                y->m_right->m_parent = y;
            }

            transplant(z, y);
            y->m_left = z->m_left;
            y->m_left->m_parent = y;
            y->m_b = z->m_b;
        }
        delete z;

        if(yColor == true)
        {
            fixDelete(x);
        }
    }

    void destroy(Node *node) noexcept
    {
        if(node != m_nil)
        {
            destroy(node->m_left);
            destroy(node->m_right);
            delete node;
        }
    }

public:
    reference operator[](const key_type &key)
    {
        Node *x = m_root;
        while(x != m_nil)
        {
            if(key < x->m_key)
            {
                x = x->m_left;
            }
            else if(x->m_key < key)
            {
                x = x->m_right;
            }
            else
            {
                return x->m_value;
            }
        }

        Node *z = new Node(key, value_type());
        Node *y = m_nil;
        x = m_root;

        while(x != m_nil)
        {
            y = x;
            x = z->m_key < x->m_key ? x->m_left : x->m_right;
        }

        z->m_parent = y;

        if(y == m_nil)
        {
            m_root = z;
        }
        else if(z->m_key < y->m_key)
        {
            y->m_left = z;
        }
        else
        {
            y->m_right = z;
        }

        z->m_left = m_nil;
        z->m_right = m_nil;
        fixInsert(z);
        return z->m_value;
    }

    struct iterator
    {
        const Node *nil;
        mutable Node *curr;

    public:
        iterator(Node *ptr = nullptr, Node *n = nullptr) noexcept : nil(n), curr(ptr) { }
        inline bool operator!=(const iterator &other) const noexcept { return curr != other.curr; }
        inline bool operator==(const iterator &other) const noexcept { return !(*this != other); }
        inline std::pair<const key_type, value_type>* operator->() const noexcept { return reinterpret_cast<std::pair<const key_type, value_type>*>(&curr->m_key); }
        inline std::pair<const key_type, value_type>& operator*() const noexcept { return reinterpret_cast<std::pair<const key_type, value_type>&>(curr->m_key); }

        iterator& operator++() noexcept
        {
            if(curr->m_right != nil)
            {
                Node *x = curr->m_right;
                while(x->m_left != nil)
                {
                    x = x->m_left;
                }
                curr = x;
            }
            else
            {
                Node *y = curr->m_parent;
                while(y != nil && curr == y->m_right)
                {
                    curr = y;
                    y = y->m_parent;
                }
                curr = y;
            }
            return *this;
        }

        iterator operator++(int) noexcept
        {
            iterator v = *this;
            ++(*this);
            return v;
        }
    };

    inline iterator begin() noexcept
    {
        Node *x = m_root;
        while(x->m_left != m_nil)
        {
            x = x->m_left;
        }
        return iterator(x, m_nil);
    }

    inline iterator end() const noexcept
    {
        return iterator(m_nil, m_nil);
    }

    TTKRedBlackTree()
    {
        m_nil = new Node(key_type(), value_type());
        m_nil->m_b = true;
        m_nil->m_left = m_nil->m_right = m_nil->m_parent = m_nil;
        m_root = m_nil;
    }

    ~TTKRedBlackTree()
    {
        destroy(m_root);
        delete m_nil;
    }

    void insert(const key_type &key, const value_type &value) noexcept
    {
        Node *z = new Node(key, value);
        Node *y = m_nil;
        Node *x = m_root;

        while(x != m_nil)
        {
            y = x;
            x = z->m_key < x->m_key ? x->m_left : x->m_right;
        }

        z->m_parent = y;

        if(y == m_nil)
        {
            m_root = z;
        }
        else if(z->m_key < y->m_key)
        {
            y->m_left = z;
        }
        else
        {
            y->m_right = z;
        }

        z->m_left = m_nil;
        z->m_right = m_nil;
        fixInsert(z);
    }

    inline void remove(const key_type &key) noexcept
    {
        Node *z = find(key);
        if(z != nullptr)
        {
            deleteNode(z);
        }
    }

    inline Node *find(const key_type &key) const noexcept
    {
        Node *x = m_root;
        while(x != m_nil)
        {
            if(key < x->m_key)
            {
                x = x->m_left;
            }
            else if(x->m_key < key)
            {
                x = x->m_right;
            }
            else
            {
                return x;
            }
        }
        return nullptr;
    }

    inline bool empty() const noexcept { return m_root == m_nil; }
    inline void clear() noexcept
    {
        destroy(m_root);
        m_root = m_nil;
    }
    // Add to RedBlackTree public section:
    inline Node *minimum(Node *x) const noexcept
    {
        while(x->m_left != m_nil)
        {
            x = x->m_left;
        }
        return x;
    }
};


//Splay Tree
template <typename _Key, typename _Value>
struct TTK_MODULE_EXPORT TTKSplayTree
{
public:
    using key_type = _Key;
    using value_type = _Value;
    using reference = _Value&;
    using const_reference = const _Value&;

private:
    struct Node
    {
        key_type m_key;
        value_type m_value;
        Node* m_left;
        Node* m_right;
        Node* m_parent;

        Node(const key_type &k, const value_type &v) noexcept
            : m_key(k),
              m_value(v),
              m_left(nullptr),
              m_right(nullptr),
              m_parent(nullptr)
        {

        }

        Node(const key_type &k, value_type &&v) noexcept
            : m_key(k),
              m_value(std::move(v)),
              m_left(nullptr),
              m_right(nullptr),
              m_parent(nullptr)
        {

        }
    };

    Node* m_root;

    inline void rotateLeft(Node *x) noexcept
    {
        Node *y = x->m_right;
        x->m_right = y->m_left;

        if(y->m_left)
        {
            y->m_left->m_parent = x;
        }
        y->m_parent = x->m_parent;

        if(x->m_parent)
        {
            m_root = y;
        }
        else if(x == x->m_parent->m_left)
        {
            x->m_parent->m_left = y;
        }
        else
        {
            x->m_parent->m_right = y;
        }

        y->m_left = x;
        x->m_parent = y;
    }

    inline void rotateRight(Node *x) noexcept
    {
        Node *y = x->m_left;
        x->m_left = y->m_right;

        if(y->m_right)
        {
            y->m_right->m_parent = x;
        }
        y->m_parent = x->m_parent;

        if(x->m_parent)
        {
            m_root = y;
        }
        else if(x == x->m_parent->m_left)
        {
            x->m_parent->m_left = y;
        }
        else
        {
            x->m_parent->m_right = y;
        }

        y->m_right = x;
        x->m_parent = y;
    }

    void splay(Node *x)
    {
        while(x->m_parent)
        {
            if(!x->m_parent->m_parent)
            {
                if(x == x->m_parent->m_left)
                {
                    rotateRight(x->m_parent);
                }
                else
                {
                    rotateLeft(x->m_parent);
                }
            }
            else if(x == x->m_parent->m_left && x->m_parent == x->m_parent->m_parent->m_left)
            {
                rotateRight(x->m_parent->m_parent);
                rotateRight(x->m_parent);
            }
            else if(x == x->m_parent->m_right && x->m_parent == x->m_parent->m_parent->m_right)
            {
                rotateLeft(x->m_parent->m_parent);
                rotateLeft(x->m_parent);
            }
            else if(x == x->m_parent->m_right && x->m_parent == x->m_parent->m_parent->m_left)
            {
                rotateLeft(x->m_parent);
                rotateRight(x->m_parent);
            }
            else
            {
                rotateRight(x->m_parent);
                rotateLeft(x->m_parent);
            }
        }
    }

    void destroy(Node *node) noexcept
    {
        if(node)
        {
            destroy(node->m_left);
            destroy(node->m_right);
            delete node;
        }
    }

public:
    TTKSplayTree()
        : m_root(nullptr)
    {

    }

    ~TTKSplayTree()
    {
        destroy(m_root);
    }

    void insert(const key_type &key, const value_type &value)
    {
        Node *z = new Node(key, value);
        Node *x = m_root;
        Node *y = nullptr;

        while(x)
        {
            y = x;
            if(key < x->m_key)
            {
                x = x->m_left;
            }
            else if(key > x->m_key)
            {
                x = x->m_right;
            }
            else
            {
                x->m_value = value;
                splay(x);
                delete z;
                return;
            }
        }

        z->m_parent = y;

        if(!y)
        {
            m_root = z;
        }
        else if(key < y->m_key)
        {
            y->m_left = z;
        }
        else
        {
            y->m_right = z;
        }

        splay(z);
    }

    inline value_type* find(const key_type &key)
    {
        Node *x = m_root;
        while(x)
        {
            if(key < x->m_key)
            {
                x = x->m_left;
            }
            else if(key > x->m_key)
            {
                x = x->m_right;
            }
            else
            {
                splay(x);
                return &x->m_value;
            }
        }
        return nullptr;
    }

    void remove(const key_type &key)
    {
        Node *x = m_root;
        while(x)
        {
            if(key < x->m_key)
            {
                x = x->m_left;
            }
            else if(key > x->m_key)
            {
                x = x->m_right;
            }
            else
            {
                break;
            }
        }

        if(!x)
        {
            return;
        }

        splay(x);

        Node *l = x->m_left;
        if(l)
        {
            l->m_parent = nullptr;
        }

        Node *r = x->m_right;
        if(r)
        {
            r->m_parent = nullptr;
        }

        delete x;

        if(!l)
        {
            m_root = r;
        }
        else
        {
            while(l->m_right)
            {
                l = l->m_right;
            }

            splay(l);
            l->m_right = r;

            if(r)
            {
                r->m_parent = l;
            }
            m_root = l;
        }
    }

    //Similar to RedBlackTree's operator[]
    reference operator[](const key_type &key)
    {
        Node *x = m_root;
        while(x)
        {
            if(key < x->m_key)
            {
                x = x->m_left;
            }
            else if(x->m_key < key)
            {
                x = x->m_right;
            }
            else
            {
                splay(x); //Splay finds the node to the root
                return x->m_value;
            }
        }
        //Not found, insert new node
        Node *z = new Node(key, value_type());
        Node *y = nullptr;
        x = m_root;

        while(x)
        {
            y = x;
            x = key < x->m_key ? x->m_left : x->m_right;
        }

        z->m_parent = y;

        if(!y)
        {
            m_root = z;
        }
        else if(key < y->m_key)
        {
            y->m_left = z;
        }
        else
        {
            y->m_right = z;
        }

        splay(z); //Splay new node to root
        return z->m_value;
    }

    // Iterator implementation
    template<typename A, typename B>
    struct iterator
    {
        Node *m_curr;
        iterator(Node *ptr = nullptr): m_curr(ptr) { }
        bool operator!=(const iterator& other) const { return m_curr != other.m_curr; }
        bool operator==(const iterator& other) const { return m_curr == other.m_curr; }
        std::pair<const A, B>* operator->() const { return reinterpret_cast<std::pair<const A, B>*>(&m_curr->m_key); }

        iterator& operator++()
        {
            if(m_curr->m_right)
            {
                m_curr = m_curr->m_right;
                while(m_curr->m_left)
                {
                    m_curr = m_curr->m_left;
                }
            }
            else
            {
                Node *y = m_curr->m_parent;
                while(y && m_curr == y->m_right)
                {
                    m_curr = y;
                    y = y->m_parent;
                }
                m_curr = y;
            }
            return *this;
        }
    };

    inline iterator<key_type, value_type> begin()
    {
        Node *x = m_root;
        if(!x)
        {
            return iterator<key_type, value_type>(nullptr);
        }

        while(x->m_left)
        {
            x = x->m_left;
        }
        return iterator<key_type, value_type>(x);
    }

    inline iterator<key_type, value_type> end()
    {
        return iterator<key_type, value_type>(nullptr);
    }

    inline bool empty() const { return m_root == nullptr; }
    inline void clear() noexcept
    {
        destroy(m_root);
        m_root = nullptr;
    }
};

#endif // TTKTREE_H
