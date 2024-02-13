#include <iostream>
#include "queue.hpp"

#define SIZE 100





int main () {
    // Queue test
    queue_t <int> *queue = new Queue <int> ();

    std::cout << "Queue:\n";
    std::cout << "\tEnqueue:\n";
    for (int i = 0; i < SIZE; i++) {
        std::cout << "\t\tPushing: " << i << "\n";
        queue->push(i);
    }

    std::cout << "\t";
    queue->show();

    std::cout << "\n\tDequeue:\n";
    while (!queue->empty()) {
        std::cout << "\t\tPopping: " << queue->first() << "\n";
        queue->pop();
    }

    std::cout << "\t";
    queue->show();
    delete queue;


    // Stack test
    queue_t <int> *stack = new Stack <int> ();

    std::cout << "Stack:\n";
    std::cout << "\tEnqueue:\n";
    for (int i = 0; i < SIZE; i++) {
        std::cout << "\t\tPushing: " << i << "\n";
        stack->push(i);
    }

    std::cout << "\t";
    stack->show();

    std::cout << "\n\tDequeue:\n";
    while (!stack->empty()) {
        std::cout << "\t\tPopping: " << stack->first() << "\n";
        stack->pop();
    }

    std::cout << "\t";
    stack->show();
    delete stack;
    
    return 0;
}