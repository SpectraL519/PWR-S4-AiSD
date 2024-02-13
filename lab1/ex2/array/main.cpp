#include <iostream>
#include "array.hpp"

#define SIZE 10





int main () {
    std::cout << "A:\n";
    Array <int> *a = new Array <int> ();
    a->show();

    for (int i = 0; i < SIZE; i++) 
        a->insert(i, 0);
    a->show();


    std::cout << "\nB:\n";
    Array <int> *b = new Array <int> ();
    b->show();

    for (int i = 0; i < SIZE; i++) 
        b->insert(i + 10, 0);
    b->show();

    std::cout << "\nMerged:\n";
    a->merge(b);
    delete(b);
    a->show();

    for (int i = 0; i < SIZE; i++) {
        std::cout << "Deleting: A[" << i << "] = " << a->at(i) << "\n";
        a->delete_at(i);
    }
    a->show();

    return 0;
}