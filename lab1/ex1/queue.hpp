#pragma once

#include <iostream>





template <typename T>
class queue_t {
    protected:
        T *array;
        int a_size;
        int a_capacity;

        void fit_capacity () {
            if (this->a_size == this->a_capacity) {
                this->a_capacity *= 2;
                this->array = (T*)realloc(this->array, this->a_capacity * sizeof(T));
            }

            // TODO ? decrease capacity
        }


    public:
        bool empty () {
            return (this->a_size == 0);
        }

        int size () {
            return this->a_size;
        }

        int capacity () {
            return this->a_capacity;
        }

        T at (int index) {
            if (index < 0 || index > this->a_size) {
                std::cout << "Error: Index out of bounds\n";
                std::exit(1);
            }

            return this->array[index];
        }

        void show () {
            if (this->empty()) {
                std::cout << "empty\n";
                return;
            }

            std::cout << "(size = " << this->a_size << ", capacity = " << this->a_capacity << ") ";
            for (int i = 0; i < this->a_size; i++) {
                std::cout << this->array[i] << " ";
            }
            std::cout << "\n";
        }

        void push (T value) {
            this->fit_capacity();
            this->array[this->a_size++] = value;
        }

        virtual void pop () = 0;
        virtual T first () = 0;
};



template <typename T>
class Queue: public queue_t <T> {
    public:
        Queue () {
            this->a_size = 0;
            this->a_capacity = 2;

            this->array = (T*)malloc(sizeof(T));
        }

        ~Queue () {
            free(this->array);
        }

        void pop () {
            int new_size = this->a_size - 1;
            for (int i = 0; i < new_size; i++) 
                this->array[i] = this->array[i + 1];
            this->a_size--;
        }

        T first () {
            return *(this->array);
        }
};



template <typename T>
class Stack: public queue_t <T> {
    public:
        Stack () {
            this->a_size = 0;
            this->a_capacity = 2;

            this->array = (T*)malloc(sizeof(T));
        }

        ~Stack () {
            free(this->array);
        }

        void pop () {
            this->a_size--;
        }

        T first () {
            return *(this->array + this->a_size - 1);
        }
};