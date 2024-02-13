#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "bst.hpp"



namespace tree {
    class splay : public bst {
        private:
            void _left_rotate (node_t* x, complexity_t &ct);
            void _right_rotate (node_t* x, complexity_t &ct);
            void _splay (node_t* x, complexity_t &ct);
            const bool _search (const int key, complexity_t &ct);

        public:
            splay() = default;
            splay (const std::vector<int>& keys);

            const bool search (const int key);
            const complexity_t insert_key (const int key);
            const complexity_t delete_key (const int key);
    };
} // namespace tree



tree::splay::splay (const std::vector<int>& keys) {
    for (const int& key : keys)
        this->insert_key(key);
}



const bool tree::splay::search (const int key) {
    if (this->empty())
        return false;

    complexity_t dummy_ct = { 0, 0, 0 };
    node_t *curr = this->root;

    while (curr != nullptr) {
        if (curr->key == key) {
            this->_splay(curr, dummy_ct);
            return true;
        }

        if (key < curr->key) 
            curr = curr->left;
        else
            curr = curr->right;
    }

    return false;
}

const tree::complexity_t tree::splay::insert_key (const int key) {
    complexity_t ct = {
        .comparisons = 1, // 79
        .ptr_ops = 2, // 67, 68
        .height = 0
    };

    node_t* z = this->root;
    node_t* p = nullptr;

    while (z != nullptr) {
        ct.comparisons++;
        ct.ptr_ops += 2;

        p = z;
        if (key < z->key)
            z = z->left;
        else
            z = z->right;
    }

    z = new node_t(key);
    z->parent = p;

    if (p == nullptr) {
        ct.ptr_ops++;
        this->root = z;
    }
    else {
        ct.comparisons++;
        ct.ptr_ops++;

        if (p->key < z->key)
            p->right = z;
        else
            p->left = z;
    }

    this->_splay(z, ct);
    this->_size++;

    return std::move(ct);
}

const tree::complexity_t tree::splay::delete_key (const int key) {
    complexity_t ct = {
        .comparisons = 0,
        .ptr_ops = 1, // 111
        .height = 0
    };

    node_t* node = this->_search_node(this->root, key, ct);

    if (node != nullptr) {
        this->_splay(node, ct);
        this->_delete_node(node, ct);
        this->_size--;
    }

    ct.height = this->_node_height(this->root);
    return std::move(ct);
}



void tree::splay::_left_rotate (node_t *x, complexity_t &ct) {
    ct.ptr_ops += 2; // 127, 156
    node_t* y = x->right;

    if (y != nullptr) {
        ct.ptr_ops += 2;

        x->right = y->left;
        if (y->left != nullptr) {
            ct.ptr_ops++;
            y->left->parent = x;
        }
        y->parent = x->parent;
    }

    if (x->parent == nullptr) {
        ct.ptr_ops++;
        root = y;
    }
    else {
        ct.ptr_ops++;
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }

    if (y != nullptr) {
        ct.ptr_ops++;
        y->left = x;
    }
    x->parent = y;
}

void tree::splay::_right_rotate (node_t *x, complexity_t &ct) {
    ct.ptr_ops += 2; // 161, 190
    node_t *y = x->left;

    if (y != nullptr) {
        ct.ptr_ops += 2;

        x->left = y->right;
        if (y->right != nullptr) {
            ct.ptr_ops++;
            y->right->parent = x;
        }
        y->parent = x->parent;
    }

    if (x->parent == nullptr) {
        ct.ptr_ops++;
        root = y;
    }
    else {
        ct.ptr_ops++;
        if (x == x->parent->left) 
            x->parent->left = y;
        else 
            x->parent->right = y;
    }

    if (y != nullptr) {
        ct.ptr_ops++;
        y->right = x;
    }
    x->parent = y;
}

void tree::splay::_splay (node_t* x, complexity_t &ct) {
    while (x->parent != nullptr) {
        if (x->parent->parent == nullptr) {
            if (x->parent->left == x)
                this->_right_rotate(x->parent, ct);
            else
                this->_left_rotate(x->parent, ct);
        }
        else if (x->parent->left == x && x->parent->parent->left == x->parent) {
            this->_right_rotate(x->parent->parent, ct);
            this->_right_rotate(x->parent, ct);
        }
        else if (x->parent->right == x && x->parent->parent->right == x->parent) {
            this->_left_rotate(x->parent->parent, ct);
            this->_left_rotate(x->parent, ct);
        }
        else if (x->parent->left == x && x->parent->parent->right == x->parent) {
            this->_right_rotate(x->parent, ct);
            this->_left_rotate(x->parent, ct);
        }
        else {
            this->_left_rotate(x->parent, ct);
            this->_right_rotate(x->parent, ct);
        }
    }
}
