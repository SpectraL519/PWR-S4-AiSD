#pragma once

#include <iostream>





template <typename T>
struct node_t {
    T value;
    struct node_t *next;
};



template <typename T>
class List {
    private:
        int l_size;
        node_t <T> *head;
        node_t <T> *tail;


    public:
        List () {
            this->l_size = 0;
            this->head = nullptr;
            this->tail = nullptr;
        }

        ~List () {
            while (!this->empty())
                this->pop_back();
        }

        bool empty () {
            return (this->head == nullptr || this->tail == nullptr);
        }
        
        int size () {
            return this->l_size;
        }

        void show () {
            if (this->empty()) {
                std::cout << "empty\n";
                return;
            }

            node_t <T> *curr = this->head;
            std::cout << "(size = " << this->l_size << ") ";
            while (curr) {
                std::cout << curr->value << " ";
                curr = curr->next;
            }
            std::cout << "\n";
        }

        void push_front (T value) {
            this->l_size++;

            node_t <T> *new_node = (node_t<T>*)malloc(sizeof(node_t<T>));
            new_node->value = value;
            new_node->next = nullptr;

            if (this->empty()) {
                this->head = new_node;
                this->tail = new_node;
                return;
            }

            new_node->next = this->head;
            this->head = new_node;
        }

        void push_back (T value) {
            this->l_size++;

            node_t <T> *new_node = (node_t<T>*)malloc(sizeof(node_t<T>));
            new_node->value = value;
            new_node->next = nullptr;

            if (this->empty()) {
                this->head = new_node;
                this->tail = new_node;
                return;
            }

            this->tail->next = new_node;
            this->tail = new_node;
        }

        bool pop_front () {
            if (this->empty())
                return false;

            node_t <T> *tmp = this->head;
            this->head = this->head->next;
            free(tmp);
            this->l_size--;
            return true;
        }

        bool pop_back () {
            if (this->empty())
                return false;

            int index = this->l_size - 1;
            node_t <T> *curr = this->head;
            for (int i = 0; i < index; i++)
                curr = curr->next;
            
            free(this->tail);
            this->tail = curr;
            this->l_size--;
            return true;

        }

        T at (int index) {
            if (index < 0 || index >= this->l_size) {
                std::cout << "Error: Index out of bounds\n";
                std::exit(1);
            }
            
            node_t <T> *curr = this->head;
            int i = 0;
            while (i++ < index) 
                curr = curr->next;
            return curr->value;
        }

        void merge (List <T> *new_list) {
            this->tail->next = new_list->head;
            this->l_size += new_list->size();
        }
};