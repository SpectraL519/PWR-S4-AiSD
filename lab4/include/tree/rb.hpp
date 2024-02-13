#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "tree.hpp"



namespace tree {
    class rb_node_t;
    static rb_node_t* RB_NONE = nullptr;
    // RB_NONE = new rb_node_t(INT32_MIN, false);

    class rb_node_t {
        public:
            bool color;
            int key;
            rb_node_t *parent = RB_NONE;
            rb_node_t *left = RB_NONE;
            rb_node_t *right = RB_NONE;

            rb_node_t (const int key, bool color = true)
            : key(key), color(color) {}
    };

    class rb : public itree {
        private:
            std::size_t _size = 0;
            std::string _left_trace;
            std::string _right_trace;

            void _display_node (const rb_node_t* node, const int depth, const char prefix);
            const std::size_t _node_height (rb_node_t* node);

            rb_node_t* _min_node (rb_node_t* node, complexity_t &ct);
            rb_node_t* _max_node (rb_node_t* node, complexity_t &ct);
            rb_node_t* _successor (rb_node_t* node, complexity_t &ct);
            rb_node_t* _search_node (rb_node_t* node, const int key, complexity_t &ct);
            
            void _left_rotate (rb_node_t* node, complexity_t &ct);
            void _right_rotate (rb_node_t* node, complexity_t &ct);
            void _insert_node (rb_node_t* node, complexity_t &ct);
            void _rb_insert (rb_node_t* node, complexity_t &ct);
            void _rb_delete (rb_node_t* node, complexity_t &ct);
            void _rb_delete_fixup (rb_node_t* node, complexity_t &ct);

        public:
            rb_node_t* root = nullptr;

            rb();
            rb (const std::vector<int>& keys);

            const std::size_t size ();
            const bool empty ();
            const std::size_t height ();

            const bool search (const int key);
            const complexity_t insert_key (const int key);
            const complexity_t delete_key (const int key);

            void display ();
    };
} // namespace tree



tree::rb::rb () {
    if (RB_NONE == nullptr)
        RB_NONE = new rb_node_t(INT32_MIN, false);
}

tree::rb::rb (const std::vector<int>& keys) {
    for (const int& key : keys) {
        this->insert_key(key);
    }
}



const std::size_t tree::rb::size () {
    return this->_size;
}

const bool tree::rb::empty () {
    return this->root == nullptr || this->root == RB_NONE;
}

const std::size_t tree::rb::height () {
    return this->_node_height(this->root);
}



const bool tree::rb::search (const int key) {
    if (this->empty())
        return false;

    rb_node_t *curr = this->root;
    while (curr != nullptr && curr != RB_NONE) {
        if (curr->key == key)
            return true;

        if (key < curr->key) 
            curr = curr->left;
        else
            curr = curr->right;
    }

    return false;
}

const tree::complexity_t tree::rb::insert_key (const int key) {
    complexity_t ct = {
        .comparisons = 0,
        .ptr_ops = 0,
        .height = 0
    };

    this->_rb_insert(new rb_node_t(key), ct);
    this->_size++;

    ct.height = this->_node_height(this->root);
    return std::move(ct);
}

const tree::complexity_t tree::rb::delete_key (const int key) {
    complexity_t ct = {
        .comparisons = 1, // 135
        .ptr_ops = 0,
        .height = 0
    };

    rb_node_t* node = this->_search_node(this->root, key, ct);

    if (node != nullptr && node != tree::RB_NONE) {
        this->_rb_delete(node, ct);
        this->_size--;
    }

    return std::move(ct);
}

void tree::rb::display () {
    this->_left_trace = std::string(this->_size, ' ');
    this->_right_trace = std::string(this->_size, ' ');
    this->_display_node(this->root, 0, '-');
}



void tree::rb::_display_node (const rb_node_t* node, const int depth, const char prefix) {
    if (node == nullptr || node == RB_NONE)
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

const std::size_t tree::rb::_node_height (rb_node_t* node) {
    if (node == nullptr || node == RB_NONE) 
        return 0;

    int leftHeight = this->_node_height(node->left);
    int rightHeight = this->_node_height(node->right);
    return std::max(leftHeight, rightHeight) + 1;
}

tree::rb_node_t* tree::rb::_min_node (rb_node_t* node, complexity_t &ct) {
    while (node->left != nullptr && node->left != RB_NONE) {
        ct.ptr_ops++;
        node = node->left;
    }
    return node;
}

tree::rb_node_t* tree::rb::_max_node (rb_node_t* node, complexity_t &ct) {
    while (node->right != nullptr && node->right != RB_NONE) {
        ct.ptr_ops++;
        node = node->right;
    }
    return node;
}

tree::rb_node_t* tree::rb::_successor (rb_node_t* node, complexity_t &ct) {
    if (node->right != nullptr && node->right != RB_NONE)
        return this->_min_node(node->right, ct);

    ct.ptr_ops++;
    rb_node_t* y = node->parent;
    while (y != nullptr && y != RB_NONE && node == y->right) {
        ct.ptr_ops += 2;
        node = y;
        y = y->parent;
    }
    
    return y;
}

tree::rb_node_t* tree::rb::_search_node (rb_node_t* node, const int key, complexity_t &ct) {
    while (node != nullptr && node != RB_NONE) {
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

void tree::rb::_left_rotate (rb_node_t* node, complexity_t &ct) {
    ct.ptr_ops += 5; // 248, 249, 256, 270, 271

    rb_node_t* y = node->right;
    node->right = y->left;
    
    if (y->left != RB_NONE && y->left != nullptr) {
        ct.ptr_ops++;
        y->left->parent = node;
    }

    y->parent = node->parent;

    if (node->parent == RB_NONE || node->parent == nullptr) {
        ct.ptr_ops++;
        root = y;   
    }
    else {
        ct.ptr_ops++;
        if (node == node->parent->left)
            node->parent->left = y;
        else
            node->parent->right = y;
    }

    y->left = node;
    node->parent = y;
}

void tree::rb::_right_rotate (rb_node_t* node, complexity_t &ct) {
    ct.ptr_ops += 5; // 277, 278, 285, 298, 299

    rb_node_t* y = node->left;
    node->left = y->right;
    
    if (y->right != RB_NONE && y->right != nullptr) {
        ct.ptr_ops++;
        y->right->parent = node;
    }

    y->parent = node->parent;
    if (node->parent == RB_NONE || node->parent == nullptr) {
        ct.ptr_ops++;
        root = y;
    }
    else {
        ct.ptr_ops++;
        if (node == node->parent->right)
            node->parent->right = y;
        else
            node->parent->left = y;
    }

    y->right = node;
    node->parent = y;
}

void tree::rb::_insert_node (rb_node_t* node, complexity_t &ct) {
    ct.ptr_ops += 3; // 305, 306, 319

    rb_node_t *y = RB_NONE;
    rb_node_t *x = root;

    while (x != RB_NONE && x != nullptr) { 
        ct.ptr_ops += 2;
        ct.comparisons++;

        y = x;
        if (node->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;
    if (y == RB_NONE || y == nullptr) {
        ct.ptr_ops++;
        root = node;
    }
    else {
        ct.comparisons++;
        ct.ptr_ops++;

        if (node->key < y->key)
            y->left = node;
        else
            y->right = node;
    }
}

void tree::rb::_rb_insert (rb_node_t* node, complexity_t &ct) {
    this->_insert_node(node, ct);

    rb_node_t* y;
    while (node != root && node->parent->color) {
        if (node->parent == node->parent->parent->left) {
            ct.ptr_ops++;
            y = node->parent->parent->right;

            if (y->color) {
                node->parent->color = false;
                y->color = false;
                node->parent->parent->color = true;

                ct.ptr_ops++;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right) {
                    ct.ptr_ops++;
                    node = node->parent;
                    this->_left_rotate(node, ct);
                }
                node->parent->color = false;
                node->parent->parent->color = true;
                this->_right_rotate(node->parent->parent, ct);
            }
        }
        else {
            ct.ptr_ops++;
            y = node->parent->parent->left;

            if (y->color) {
                node->parent->color = false;
                y->color = false;
                node->parent->parent->color = true;

                ct.ptr_ops++;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left) {
                    ct.ptr_ops++;
                    node = node->parent;
                    this->_right_rotate(node, ct);
                }
                node->parent->color = false;
                node->parent->parent->color = true;
                this->_left_rotate(node->parent->parent, ct);
            }
        }
    }

    root->color = false;
}

void tree::rb::_rb_delete (rb_node_t* node, complexity_t &ct) {
    ct.ptr_ops += 3; // 398 or 400, 403 or 405, 407

    rb_node_t* x;
    rb_node_t* y;

    if (node->left == RB_NONE || node->right == RB_NONE)
        y = node;
    else
        y = this->_successor(node, ct);

    if (y->left != RB_NONE)
        x = y->left; 
    else 
        x = y->right;

    x->parent = y->parent;
    if (y->parent == RB_NONE) {
        ct.ptr_ops++;
        root = x;
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

    if (!y->color)
        this->_rb_delete_fixup(x, ct);

    delete y;
}

void tree::rb::_rb_delete_fixup (rb_node_t* node, complexity_t &ct) {
    rb_node_t* w;

    while (node != this->root && !node->color) {
        if (node == node->parent->left) {
            ct.ptr_ops++;
            w = node->parent->right;

            if(w->color) {
                w->color = false;
                node->parent->color = true;
                this->_left_rotate(node->parent, ct);
                ct.ptr_ops++;
                w = node->parent->right;
            }

            if (!w->left->color && !w->right->color) {
                w->color = true;
                ct.ptr_ops++;
                node = node->parent;
            }
            else {
                if (!w->right->color) {
                    w->left->color = false;
                    w->color = true;
                    this->_right_rotate(w, ct);
                    w = node->parent->right;
                }
                w->color = node->parent->color;
                node->parent->color = false;
                w->right->color = false;
                this->_left_rotate(node->parent, ct);
                ct.ptr_ops++;
                node = root;
            }
        }
        else {
            ct.ptr_ops++;
            w = node->parent->left;

            if (w->color) {
                w->color = false;
                node->parent->color = true;
                this->_right_rotate(node->parent, ct);
                ct.ptr_ops++;
                w = node->parent->left;
            }

            if (!w->left->color && !w->right->color) {
                w->color = true;
                ct.ptr_ops++;
                node = node->parent;
            }
            else {
                if (!w->left->color) {
                    w->right->color = false;
                    w->color = true;
                    this->_left_rotate(w, ct);
                    ct.ptr_ops++;
                    w = node->parent->left;
                }
                w->color = node->parent->color;
                node->parent->color = false;
                w->left->color = false;
                this->_left_rotate(node->parent, ct);
                ct.ptr_ops++;
                node = root;
            }
        }
    }

    node->color = false;
}