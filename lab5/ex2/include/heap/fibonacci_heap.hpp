#pragma once

#include <stdexcept>
#include <optional>
#include <algorithm>
#include <cstdio>

#include <comparable.hpp>
#include <heap/heap.hpp>



namespace heap {
    template <comparable::weakly_ordered T>
    class fibonacci_heap : public i_heap<T> {
        private:
            struct node_t {
                T key;
                std::size_t degree = 0;
                bool marked = false;

                node_t* parent = nullptr;
                node_t* child = nullptr;
                node_t* prev = this;
                node_t* next = this;
            };

            node_t* _min = nullptr;
            std::size_t _size = 0;

            node_t* _heap_link (node_t* x, node_t* y);
            void _init_child (node_t* parent, node_t* child);
            node_t* _extract_min (node_t* node);

        public:
            fibonacci_heap() = default;
            ~fibonacci_heap() = default;

            std::size_t size() override;
            bool empty() override;

            void insert_key (const T key) override;
            std::optional<T> min_key() override;
            std::optional<T> extract_min() override;

            void merge (const i_heap<T>& h_prim) override;

            void show() override;
    };
} // namespace heap



// private members

template <comparable::weakly_ordered T>
heap::fibonacci_heap<T>::node_t* heap::fibonacci_heap<T>::_heap_link (node_t* x, node_t* y) {
    if (x == nullptr)
        return y;

    if (y == nullptr)
        return x;

    if (x->key > y->key)
        std::swap(x, y);

    node_t* x_next = x->next;
    node_t* y_prev = y->prev;

    x->next->prev = y->prev;
    y->prev->next = x->next;

    x->next = y;
    y->prev = x;

    return x;
}

template <comparable::weakly_ordered T>
void heap::fibonacci_heap<T>::_init_child (node_t* parent, node_t* child) {
    child->prev = child->next=child;
	child->parent = parent;
	parent->degree++;
	parent->child = this->_heap_link(parent->child, child);
}

template <comparable::weakly_ordered T>
heap::fibonacci_heap<T>::node_t* heap::fibonacci_heap<T>::_extract_min (node_t* node) {
    // set all children's mark to false and parent to null
    if (node != nullptr && node->child != nullptr) {
        node_t* child = node->child;
		do {
			child->marked = false;
			child->parent = nullptr;
			child = child->next;
		} while (child != node->child);
    }

    if (node->next == node) {
        node = node->child;
    } 
    else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
        node = this->_heap_link(node->next, node->child);
    }
    
    if (node == nullptr) 
        return node;

    node_t* trees[64];
    std::fill(trees, trees + 64, nullptr);
    
    while (true) {
        if (trees[node->degree] != nullptr) {
            node_t* root = trees[node->degree];

            if (root == node)
                break;

            trees[node->degree] = nullptr;

            if (node->key < root->key) {
                root->prev->next = root->next;
                root->next->prev = root->prev;
                this->_init_child(node, root);
            } 
            else {
                root->prev->next = root->next;
                root->next->prev = root->prev;

                if (node->next == node) {
                    root->next = root->prev = root;
                } 
                else {
                    node->prev->next = root;
                    node->next->prev = root;
                    root->next = node->next;
                    root->prev = node->prev;
                }
                this->_init_child(root, node);
                node = root;
            }
            continue;
        } 
        else {
            trees[node->degree] = node;
        }
        node = node->next;
    }

    node_t* min = node;
    node_t* tmp = node;
    do {
        if (node->key < min->key)
            min = node;

        node = node->next;
    } while (node != tmp);
    
    return min;
}



// public members

template <comparable::weakly_ordered T>
std::size_t heap::fibonacci_heap<T>::size () {
    return this->_size;
}

template <comparable::weakly_ordered T>
bool heap::fibonacci_heap<T>::empty () {
    return this->_min == nullptr;
}

template <comparable::weakly_ordered T>
void heap::fibonacci_heap<T>::insert_key (T key) {
    node_t* new_node = new node_t { .key = key };
    this->_min = this->_heap_link(this->_min, new_node);
    this->_size++;
}

template <comparable::weakly_ordered T>
std::optional<T> heap::fibonacci_heap<T>::min_key () {
    if (this->_min == nullptr)
        return std::nullopt;

    return this->_min->key;
}

template <comparable::weakly_ordered T>
std::optional<T> heap::fibonacci_heap<T>::extract_min () {
    if (this->_min == nullptr)
        return std::nullopt;

    // store the current minimum node and key
    node_t* old_min = this->_min;
    T old_min_key = old_min->key;

    // extract min value node
    this->_min = this->_extract_min(this->_min);

    delete old_min;
    this->_size--;

    return old_min_key;
}

template <comparable::weakly_ordered T>
void heap::fibonacci_heap<T>::merge (const i_heap<T>& h_prim) {
    const fibonacci_heap<T>* h_prim_ptr = dynamic_cast<const fibonacci_heap<T>*>(&h_prim);

    if (h_prim_ptr == nullptr)
        throw std::invalid_argument("Error: cannot perform a union operation between different types of heap!");

    const fibonacci_heap<T> fibh_prim = *h_prim_ptr;

    this->_min = this->_heap_link(this->_min, fibh_prim._min);
    this->_size += fibh_prim._size;
}

template <comparable::weakly_ordered T>
void heap::fibonacci_heap<T>::show () {
    if (this->_min == nullptr) {
        std::cout << "empty heap" << std::endl;
        return;
    }

    std::size_t num_roots = 0;
    if (this->_min != nullptr) {
        node_t* root = this->_min;
        do {
            num_roots++;
            root = root->next;
        } while (root != this->_min);
    }

    std::cout << "num_roots = " << num_roots << std::endl;
}