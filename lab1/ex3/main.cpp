#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "list.hpp"

#define SIZE 10000
#define RANGE 100
#define MERGE_SIZE 10





int main (int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Error: Invalid arguments\n";
        std::exit(1);
    }

    std::string arg = argv[1];

    if (arg == "time") {
        std::mt19937 generator;
        std::uniform_int_distribution <int> uniform;
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);

        std::cout << "Reference time test:\n";

        List <int> *list = new List <int> ();
        for (int i = 0; i < SIZE; i++) 
            list->push_back(uniform(generator) % RANGE);
        

        std::vector <int> index(5);
        for (int i = 0; i < 5; i++) 
            index[i] = pow(10, i) - 1;

        for (auto idx : index) {
            const int n = 100;
            double cpu_time_total = 0.0;
            clock_t start, end;

            for (int i = 0; i < n; i++) {
                start = clock();
                int value = list->at(idx);
                end = clock();

                cpu_time_total += (double)(end - start);
            }
            double cpu_time_avg = cpu_time_total / (double)n;
            std::cout << "\tAverage execution time of `list->at(" << idx << ") = " << cpu_time_avg << "\n";
        }

        const int n = 100;
        double cpu_time_total = 0.0;
        clock_t start, end;

        for (int i = 0; i < n; i++) {
            int random_idx = uniform(generator) % SIZE;
            start = clock();
            int value = list->at(random_idx);
            end = clock();

            cpu_time_total += (double)(end - start);
        }
        double cpu_time_avg = cpu_time_total / (double)n;
        std::cout << "\tAverage execution time of `list->at(random_index) = " << cpu_time_avg << "\n";

        return 0;
    }


    if (arg == "merge") {
        std::cout << "Merge testing\n";
        List <int> *A = new List <int> ();
        List <int> *B = new List <int> ();
        for (int i = 0; i < MERGE_SIZE; i++) {
            A->push_front(i);
            B->push_back(i);
        }

        std::cout << "A: ";
        A->show();
        std::cout << "B: ";
        B->show();
        
        A->merge(B);
        std::cout << "Merged: ";
        A->show();
        return 0;
    }

    std::cout << "Error: Invalid argument\n";
    return 1;
}