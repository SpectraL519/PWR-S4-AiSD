#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include "sort.hpp"

#define DEBUG 40





std::string file_name (std::string how) {
    return ("results/" + how + ".csv");
}



int main (int argc, char* argv[]) {
    // running the whole script:
    // ./generate <n> <how> | ./sort <how>

    // Init variables
    if (argc < 2) {
        std::cerr << "Error: Invalid arguments\n";
        std::cerr << "\tScript invocation syntax: `./sort <how>`";
        return 1;
    }

    std::string how = argv[1];
    std::function <sort::sort_t(std::vector<int>&, bool)> sort;

    if (how == "insert") 
        sort = sort::insert_sort<int>;
    else if (how == "merge")
        sort = sort::merge_sort<int>;
    else if (how == "quick")
        sort = sort::quick_sort<int>;
    else if (how == "dp_quick")
        sort = sort::dual_pivot_quick_sort<int>;
    else if (how == "tim")
        sort = sort::tim_sort<int>;
    else {
        std::cerr << "Error: Invalid value of `how` - must be ['insert', 'merge', 'quick']!\n";
        std::exit(1);
    }

    int arr_size;
    std::cin >> arr_size;
    bool debug = (arr_size < DEBUG);
    if (argc == 3) 
        if (std::string(argv[2]) == "!debug")
            debug = false;

    if (!std::cin.good()) {
        std::cerr << "Error: Invalid input!\n";
        std::exit(1);
    }

    std::vector <int> array(arr_size);
    for (int i = 0; i < arr_size; i++)
        std::cin >> array[i];


    // Sort the array
    if (debug) {
        std::cout << "Input array:\n\t";
        show(array);
        std::cout << "\n";
    }

    sort::sort_t st = sort(array, debug);

    if (debug) {
        std::cout << "\nSorted array:\n\t";
        show(array);
        std::cout << "\n";
    }
    std::cout << "Comparisons: " << st.comparisons << "\n";
    std::cout << "Key swaps: " << st.swaps << "\n";


    // Export results
    try {
        std::ofstream out_file;
        out_file.open(file_name(how), std::ios::app);
        out_file << array.size() << "," << st.comparisons << "," << st.swaps << "\n";
    }
    catch (std::exception& e) {
        std::cout << "Error: Could not read '" << file_name << "'!\n\t";
        std::cout << e.what() << "\n";
        std::exit(1);
    }

    return 0;
}