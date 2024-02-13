#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include "select.hpp"





namespace {
    static std::size_t comparisons = 0;

    std::string file_name (std::string variant) {
        return "data/search_" + variant + ".csv";
    }
}

template <comparable T>
bool bin_search (std::vector <T> array, std::size_t p, std::size_t q, T element) {
    std::size_t size = q - p + 1;
    if (size < 1) 
        return false;

    std::size_t m = p + (size / 2);
    if (++comparisons && array[m] == element) 
        return true;
    if (++comparisons && array[m] > element)
        return bin_search(array, p, m - 1, element);
    return bin_search(array, m + 1, q, element);
}



int main (int argc, char* argv[]) {
    std::string variant = "random";
    if (argc >= 2)
        variant = argv[1];

    int arr_size, element;
    std::cin >> arr_size >> element;

    std::vector <int> array(arr_size);
    for (int i = 0; i < arr_size; i++)
        std::cin >> array[i];

    std::cout << "Input array: ";
    for (int x : array)
        std::cout << x << " ";
        
    std::sort(array.begin(), array.end());
    std::cout << "\nSorted: ";
    for (int x : array)
        std::cout << x << " ";

    std::cout << "\nSearching for: " << element << "\n";
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Found: " << bin_search(array, 0, array.size() - 1, element) << "\n";
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    float exec_time = (float)duration.count() / 1000.f;

    try {
        std::ofstream out_file;
        out_file.open(file_name(variant), std::ios::app);
        out_file << array.size() << "," << comparisons << "," << exec_time << "\n";
    }
    catch (std::exception& e) {
        std::cout << "Error: Could not open the result file!\n\t";
        std::cout << e.what() << "\n";
        std::exit(1);
    }
    
    return 0;
}