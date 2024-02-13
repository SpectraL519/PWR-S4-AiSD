#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include "select.hpp"

#define DEBUG 40





std::string file_name (std::string how, std::string sub_arr_size_str) {
    if (sub_arr_size_str == "default")
        return "data/" + how + ".csv";
    return ("data/" + how + "_" + sub_arr_size_str + ".csv");
}



int main (int argc, char* argv[]) {
    // running the whole script:
    // ./generate <n> <how> | ./select <how> <sub_arr_size_str>

    // Init variables
    std::string how;
    std::string sub_arr_size_str = "default";
    if (argc < 2) 
        how = "random";
    else {
        how = argv[1];
        if (argc >= 3)
            sub_arr_size_str = argv[2];
    }

    std::function <std::pair<int, select::select_t>(std::vector<int>,std::size_t, bool)> select;

    if (how == "random") 
        select = select::random_select<int>;
    else if (how == "median")
        select = select::median_select<int>;
    else {
        std::cerr << "Error: Invalid value of `how` - must be ['insert', 'merge', 'quick']!\n";
        std::exit(1);
    }

    std::size_t subarr_size, pos;
    try {
        if (sub_arr_size_str != "default") {
            subarr_size = std::stoi(sub_arr_size_str, &pos);
            if (pos < sub_arr_size_str.size()) {
                std::cerr << "Trailing characters after number: " << sub_arr_size_str << '\n';
                std::exit(1);
            }
        }
    }
    catch (std::invalid_argument const &ex) {
        std::cerr << "Invalid number: " << sub_arr_size_str << '\n';
        std::exit(1);
    } 
    select::subarray_size = subarr_size;

    int arr_size;
    std::size_t k;
    std::cin >> arr_size >> k;
    bool debug = (arr_size < DEBUG);
    if (std::string(argv[argc - 1]) == "!debug")
        debug = false;

    if (!std::cin.good()) {
        std::cerr << "Error: Invalid input!\n";
        std::exit(1);
    }

    std::vector <int> array(arr_size);
    for (int i = 0; i < arr_size; i++)
        std::cin >> array[i];


    // select the array
    if (debug) {
        std::cout << "Input array:\n";
        show(array, 0, array.size() - 1);
        std::cout << "Statistic (k): " << k << "\n\n\n";
    }

    auto [kth_statistic, st] = select(array, k, debug);
    std::cout << "\nkth statistic: " << kth_statistic << "\n";
    std::cout << "Comparisons: " << st.comparisons << "\n";
    std::cout << "Key swaps: " << st.swaps << "\n";


    // Export results
    try {
        std::ofstream out_file;
        out_file.open(file_name(how, sub_arr_size_str), std::ios::app);
        out_file << array.size() << "," << st.comparisons << "," << st.swaps << "\n";
    }
    catch (std::exception& e) {
        std::cout << "Error: Could not open the result file'!\n\t";
        std::cout << e.what() << "\n";
        std::exit(1);
    }

    return 0;
}