#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "tree.hpp"



namespace tree {
    class node_t {
        public:
            int key;
            node_t *parent = nullptr;
            node_t *left = nullptr;
            node_t *right = nullptr;

            node_t (const int key) : key(key) {}
    };

    class bst : public itree {
        protected:
            std::size_t _size = 0;
            std::string _left_trace;
            std::string _right_trace;

            void _display_node (const node_t* node, const int depth, const char prefix);
            const std::size_t _node_height (node_t* node);
            void _delete_node (node_t* node, complexity_t &ct);

            node_t* _min_node (node_t* node, complexity_t &ct);
            node_t* _max_node (node_t* node, complexity_t &ct);
            node_t* _successor (node_t* node, complexity_t &ct);
            node_t* _search_node (node_t* node, const int key, complexity_t &ct);

        public:
            node_t* root = nullptr;

            bst() = default;
            bst (const std::vector<int>& keys);

            const std::size_t size ();
            const bool empty ();
            const std::size_t height ();

            virtual const bool search (const int key);
            virtual const complexity_t insert_key (const int key);
            virtual const complexity_t delete_key (const int key);

            void display ();
    };
} // namespace tree



tree::bst::bst (const std::vector<int>& keys) {
    for (const int& key : keys) {
        this->insert_key(key);
    }
}



const std::size_t tree::bst::size () {
    return this->_size;
}

const bool tree::bst::empty () {
    return this->root == nullptr;
}

const std::size_t tree::bst::height () {
    return this->_node_height(this->root);
}



const bool tree::bst::search (const int key) {
    if (this->empty())
        return false;

    node_t *curr = this->root;
    while (curr != nullptr) {
        if (curr->key == key)
            return true;

        if (key < curr->key) 
            curr = curr->left;
        else
            curr = curr->right;
    }

    return false;
}

const tree::complexity_t tree::bst::insert_key (const int key) {
    complexity_t ct = {
        .comparisons = 0,
        .ptr_ops = 3, // 105, 106, 119
        .height = 0
    };

    node_t *new_node = new node_t(key);
    node_t *y = nullptr;
    node_t *x = this->root;

    while (x != nullptr) {
        ct.comparisons++;
        ct.ptr_ops += 2;

        y = x;
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    new_node->parent = y;
    if (y == nullptr) {
        ct.ptr_ops++;
        this->root = new_node;
    }
    else {
        ct.comparisons++;
        ct.ptr_ops++;

        if (key < y->key)
            y->left = new_node;
        else
            y->right = new_node;
    }

    this->_size++;
    
    ct.height = this->_node_height(this->root);
    return std::move(ct);
}

const tree::complexity_t tree::bst::delete_key (const int key) {
    complexity_t ct = {
        .comparisons = 0,
        .ptr_ops = 1, // 147
        .height = 0
    };

    node_t* node = this->_search_node(this->root, key, ct);

    if (node != nullptr) {
        this->_delete_node(node, ct);
        this->_size--;
    }

    ct.height = this->_node_height(this->root);
    return std::move(ct);
}

void tree::bst::display () {
    this->_left_trace = std::string(this->_size, ' ');
    this->_right_trace = std::string(this->_size, ' ');
    this->_display_node(this->root, 0, '-');
}



void tree::bst::_display_node (const node_t* node, const int depth, const char prefix) {
    if (node == nullptr)
        return;

    if (node->left != nullptr)
        this->_display_node(node->left, depth + 1, '/');

    if (prefix == '/')
        this->_left_trace[depth - 1] = '|';
    else if (prefix == '\\')
        this->_right_trace[depth - 1] = ' ';

    if (depth == 0)
        printf("-");
    else if (depth > 0)
        printf(" ");

    for (int i = 0; i < depth - 1; i++)
        if (this->_left_trace[i] == '|' || this->_right_trace[i] == '|')
            printf("| ");
        else
            printf("  ");

    if (depth > 0)
        printf("%c-", prefix);

    printf("[%d]\n", node->key);
    this->_left_trace[depth] = ' ';

    if (node->right != NULL) {
        this->_right_trace[depth] = '|';
        this->_display_node(node->right, depth + 1, '\\');
    }
}

const std::size_t tree::bst::_node_height (node_t* node) {
    if (node == nullptr) 
        return 0;

    int leftHeight = this->_node_height(node->left);
    int rightHeight = this->_node_height(node->right);
    return std::max(leftHeight, rightHeight) + 1;
}

void tree::bst::_delete_node (node_t* node, complexity_t &ct) {
    ct.ptr_ops += 2; // 217 or 219, 222 or 224

    node_t* x;
    node_t* y;

    if (node->left == nullptr || node->right == nullptr) 
        y = node;
    else
        y = this->_successor(node, ct);

    if (y->left != nullptr)
        x = y->left;
    else
        x = y->right;


    if (x != nullptr) {
        ct.ptr_ops++;
        x->parent = y->parent;
    }

    if (y->parent == nullptr) {
        ct.ptr_ops++;
        this->root = x;
    }
    else {
        ct.ptr_ops++;
        if (y == y->parent->left)
            y->parent->left = x;
        else 
            y->parent->right = x;
    }

    if (y != node) 
        node->key = y->key;
}



tree::node_t* tree::bst::_min_node (node_t* node, complexity_t &ct) {
    while (node->left != nullptr) {
        ct.ptr_ops++;
        node = node->left;
    }
    return node;
}

tree::node_t* tree::bst::_max_node (node_t* node, complexity_t &ct) {
    while (node->right != nullptr) {
        ct.ptr_ops++;
        node = node->right;
    }
    return node;
}

tree::node_t* tree::bst::_successor (node_t* node, complexity_t &ct) {
    if (node->right != nullptr)
        return this->_min_node(node->right, ct);

    ct.ptr_ops++;
    node_t* y = node->parent;
    while (y != nullptr && node == y->right) {
        ct.ptr_ops += 2;
        node = y;
        y = y->parent;
    }
    
    return y;
}

tree::node_t* tree::bst::_search_node (node_t* node, const int key, complexity_t &ct) {
    while (node != nullptr) {
        ct.comparisons++;
        if (key == node->key)
            break;

        ct.comparisons++;
        ct.ptr_ops++;
        if (key < node->key)
            node = node->left;
        else 
            node = node->right;
    }

    return node;
}
