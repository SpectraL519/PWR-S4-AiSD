#pragma once

#include <iostream>
#include <malloc.h>





template <typename T>
class Array {
    private:
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
        Array () {
            this->a_size = 0;
            this->a_capacity = 2;

            this->array = (T*)malloc(sizeof(T));
        }

        ~Array () {
            free(this->array);
        }

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

        void append (T value) {
            this->fit_capacity();
            this->array[this->a_size++] = value;
        }

        bool insert (T value, int index) {
            if (index < 0 || index > this->a_size)
                return false;

            this->fit_capacity();

            for (int i = this->a_size - 1; i >= index; i--) 
                this->array[i + 1] = this->array[i];
            this->array[index] = value;
            this->a_size++;
            return true;
        }

        bool delete_at (int index) {
            if (index < 0 || index > this->a_size)
                return false;
            
            for (int i = index; i < this->a_size - 1; i++) 
                this->array[i] = this->array[i + 1];
            this->a_size--;
            return true;
        }

        void merge (Array <T> *new_array) {
            int new_size = new_array->size();
            for (int i = 0; i < new_size; i++) 
                this->append(new_array->at(i));
        }
};