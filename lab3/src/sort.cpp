#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>

#include "select.hpp"
#include "sort.hpp"





namespace {
    std::string file_name (std::string how, bool select) {
        if (select)
            return "data/sort_" + how + "_select.csv";
        return "data/sort_" + how + ".csv";
    }
}



int main (int argc, char* argv[]) {
    std::string how, use_select_str;
    bool use_select = false;

    if (argc < 2) 
        how = "quick";
    else {
        how = argv[1];
        if (argc >= 3) {
            use_select_str = argv[2];
            if (use_select_str == "use-select")
                use_select = true;
        } 
    }
    
    std::function <select::select_t(std::vector<int>&, bool, bool)> sort;

    if (how == "quick")
        sort = sort::quick_sort<int>;
    else if (how == "dp_quick")
        sort = sort::dual_pivot_quick_sort<int>;
    else {
        std::cerr << "Error: Invalid value of `how` - must be ['insert', 'merge', 'quick']!\n";
        std::exit(1);
    }

    int arr_size;
    std::cin >> arr_size;
    bool debug = (arr_size <= 40);

    std::vector <int> array(arr_size);
    for (int i = 0; i < arr_size; i++)
        std::cin >> array[i];

    std::cout << "Input array: ";
    for (int x : array)
        std::cout << x << " ";
    std::cout << "\n\n";

    select::select_t st = sort(array, debug, use_select);

    std::cout << "\nSorted array: ";
    for (int x : array)
        std::cout << x << " ";
    std::cout << "\n";
    std::cout << "Comparisons: " << st.comparisons << "\n";
    std::cout << "Key swaps: " << st.swaps << "\n";

    // Export results
    try {
        std::ofstream out_file;
        out_file.open(file_name(how, use_select), std::ios::app);
        out_file << array.size() << "," << st.comparisons << "," << st.swaps << "\n";
    }
    catch (std::exception& e) {
        std::cout << "Error: Could not open the result file'!\n\t";
        std::cout << e.what() << "\n";
        std::exit(1);
    }

    return 0;
}