#pragma once

#include <stdexcept>
#include <optional>
#include <stack>
#include <queue>

#include <comparable.hpp>
#include <heap/heap.hpp>



static std::size_t bin_cmp = 0;

namespace heap {
    template <comparable::weakly_ordered T>
    class binomial_heap : public i_heap<T> {
        private:
            struct node_t {
                T key;
                std::size_t degree = 0;

                node_t* parent = nullptr;
                node_t* child = nullptr;
                node_t* sibling = nullptr;
            };

            node_t* _head = nullptr;
            std::size_t _size = 0;

            binomial_heap (node_t* head);
            void _heap_link (node_t* x, node_t* y);
            void _print_tree (node_t* root);

        public:
            binomial_heap() = default;
            ~binomial_heap() = default;

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
heap::binomial_heap<T>::binomial_heap (node_t* head) {
    this->_head = head;
}

template <comparable::weakly_ordered T>
void heap::binomial_heap<T>::_heap_link (node_t* x, node_t* y) {
    if (x->degree != y->degree)
        throw std::invalid_argument(
            "Error: node_ts x and y must have the same degree (" 
            + std::to_string(x->degree) 
            + " != " 
            + std::to_string(y->degree) 
            + ")!"
        );

    y->parent = x;
    y->sibling = x->child;
    x->child = y;
    x->degree++;
}


// public members

template <comparable::weakly_ordered T>
std::size_t heap::binomial_heap<T>::size () {
    return this->_size;
}

template <comparable::weakly_ordered T>
bool heap::binomial_heap<T>::empty () {
    return this->_head == nullptr;
}

template <comparable::weakly_ordered T>
void heap::binomial_heap<T>::insert_key (const T key) {
    binomial_heap<T> h_prim(new node_t { .key = key });
    this->merge(h_prim);
    this->_size++;
}

template <comparable::weakly_ordered T>
std::optional<T> heap::binomial_heap<T>::min_key () {
    if (this->_head == nullptr)
        return std::nullopt;

    node_t* curr = this->_head;
    T min_key = this->_head->key;

    while (curr != nullptr) {
        bin_cmp++;
        if (curr->key < min_key) 
            min_key = curr->key;

        curr = curr->sibling;
    }

    return min_key;
}

template <comparable::weakly_ordered T>
std::optional<T> heap::binomial_heap<T>::extract_min () {
    if (this->_head == nullptr)
        return std::nullopt;

    // find a root with min key value
    node_t* curr = this->_head;
    node_t* prev = nullptr;

    T min_key = this->_head->key;
    node_t* min_node = nullptr;
    node_t* min_node_prev = nullptr;

    while (curr != nullptr) {
        bin_cmp++;
        if (curr->key <= min_key) {
            min_key = curr->key;
            min_node = curr;
            min_node_prev = prev;
        }

        prev = curr;
        curr = curr->sibling;
    }

    // delete the min value root - delete sibling reference
    if (min_node_prev != nullptr)
        min_node_prev->sibling = min_node->sibling;
    else 
        this->_head = min_node->sibling;

    // delete all min node parent references
    if (min_node->child != nullptr) {
        std::stack <node_t*> s; // used to reverse the order of min node's children
        node_t* child = min_node->child;
        while (child != nullptr) {
            s.push(child);

            child->parent = nullptr;
            child = child->sibling;
        }

        // reverse the order of min node's children
        node_t* h_prim_head = s.top();
        s.pop();
        curr = h_prim_head;

        while (!s.empty()) {
            curr->sibling = s.top();
            s.pop();
            curr = curr->sibling;
        }
        curr->sibling = nullptr;

        // merge h_prim into the heap
        binomial_heap<T> h_prim(h_prim_head);
        this->merge(h_prim);
    }

    this->_size--;
    return min_key;
}

template <comparable::weakly_ordered T>
void heap::binomial_heap<T>::merge (const i_heap<T>& h_prim) {
    const binomial_heap<T>* h_prim_ptr = dynamic_cast<const binomial_heap<T>*>(&h_prim);

    if (h_prim_ptr == nullptr)
        throw std::invalid_argument("Error: cannot perform a union operation between different types of heap!");

    const binomial_heap<T> binh_prim = *h_prim_ptr;

    if (this->_head == nullptr) {
        this->_head = binh_prim._head;
        return;
    }

    node_t* curr1 = this->_head;
    node_t* curr2 = binh_prim._head;
    node_t* curr3 = nullptr;
    node_t* tmp = nullptr;

    if (curr1->degree <= curr2->degree) {
        curr3 = curr1;
        curr1 = curr1->sibling;
    }
    else {
        curr3 = curr2;
        curr2 = curr2->sibling;
    }
    tmp = curr3;

    // merge heaps without taking care of trees with equal degrees
    while (curr1 != nullptr && curr2 != nullptr) {
        if (curr1->degree <= curr2->degree) {
            curr3->sibling = curr1;
            curr1 = curr1->sibling;
        }
        else {
            curr3->sibling = curr2;
            curr2 = curr2->sibling;
        }
        curr3 = curr3->sibling;
    }

    // move the remaining trees from h and h' to the new heap
    while (curr1 != nullptr) {
        curr3->sibling = curr1;
        curr1 = curr1->sibling;
        curr3 = curr3->sibling;
    }
    while (curr2 != nullptr) {
        curr3->sibling = curr2;
        curr2 = curr2->sibling;
        curr3 = curr3->sibling;
    }

    // merge trees with equal degrees
    curr3 = tmp;
    node_t* prev = nullptr;
    node_t* next = curr3->sibling;

    while (next != nullptr) {
        if (
            (curr3->degree != next->degree) || // 2 adjacent roots have different degrees
            (next->sibling != nullptr && curr3->degree == next->sibling->degree) // 3 consecutive roots have equal degrees
        ) { // move to the next tree
            prev = curr3;
            curr3 = next;
        }
        else { // merge trees
            bin_cmp++;
            if (curr3->key <= next->key) {
                curr3->sibling = next->sibling;
                this->_heap_link(curr3, next);
            }
            else {
                if (prev == nullptr)
                    tmp = next;
                else
                    prev->sibling = next;

                this->_heap_link(next, curr3);
                curr3 = next;
            }
        }

        next = curr3->sibling;
    }

    this->_head = tmp;
}



template <comparable::weakly_ordered T>
void heap::binomial_heap<T>::_print_tree (node_t* root){
    if (root == nullptr) {
        std::cout << "none" << std::endl;
        return;
    }
    
    std::queue<node_t*> q;
    q.push(root);

    while (!q.empty()) {
        node_t* node = q.front();
        q.pop();
        std::cout << node->key << " ";

        if (node->child != nullptr) {
            node_t* temp = node->child;
            while (temp != nullptr) {
                q.push(temp);
                temp = temp->sibling;
            }
        } 
    }
}

template <comparable::weakly_ordered T>
void heap::binomial_heap<T>::show () {
    if (this->_head == nullptr) {
        std::cout << "empty heap" << std::endl << std::endl;
        return;
    }


    node_t* curr = this->_head;
    while (curr != nullptr) {
        std::cout << "B"<< curr->degree << std::endl;
        std::cout << "Nodes in this binomial tree (" << pow(2, curr->degree) << "): ";

        this->_print_tree(curr);
        curr = curr->sibling;

        std::cout << std::endl << std::endl;
    }
}