#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <random>
#include <chrono>
#include <algorithm>



int main (int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Error: Invalid arguments\n";
        std::cerr << "\tScript invocation syntax: `./generate <n> <how> <k>`\n";
        return 1;
    }

    std::size_t arr_size;
    int k;
    std::vector <int> array;

    std::string arr_size_str = argv[1];
    std::string how = argv[2];
    std::string k_str = argv[3];

    std::mt19937 generator; // mersenne twister
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
    std::uniform_int_distribution <int> uniform;

    try {
        std::size_t as_pos;
        arr_size = std::stoi(arr_size_str, &as_pos);
        if (as_pos < arr_size_str.size()) {
            std::cerr << "Trailing characters after number: " << arr_size_str << '\n';
            std::exit(1);
        }
        array.resize(arr_size, 0);
        uniform.param(std::uniform_int_distribution<int>::param_type(0, (2 * arr_size) - 1));

        if (k_str == "random") 
            k = uniform(generator) % arr_size + 1;
        else if (k_str == "search") 
            k = uniform(generator);
        else if (k_str == "none")
            k = NULL;
        else {
            std::size_t k_pos;
            k = std::stoi(k_str, &k_pos);
            if (as_pos < arr_size_str.size()) {
                std::cerr << "Trailing characters after number: " << k_str << '\n';
                std::exit(1);
            }
        }
    } 
    catch (std::invalid_argument const &ex) {
        std::cerr << "Invalid number: " << arr_size_str << '\n';
        std::exit(1);
    } 
    catch (std::out_of_range const &ex) {
        std::cerr << "Number out of range: " << arr_size_str << '\n';
        std::exit(1);
    }

    if (how == "random") {
        auto get_random = [&uniform, &generator] () {
            return uniform(generator);
        };

        std::generate(array.begin(), array.end(), get_random);
    }
    else if (how == "ascending") {
        std::iota(array.begin(), array.end(), 0);
    }
    else if (how == "descending") {
        std::iota(std::rbegin(array), std::rend(array), 0);
    }
    else {
        std::cerr << "Error: Invalid value of `how` - must be ['random', 'ascending', 'descending']!\n";
        std::exit(1);
    }

    std::cout << arr_size << "\n";
    if (k != NULL)
        std::cout << k << "\n";
    for (int a : array)
        std::cout << a << " ";
    std::cout << "\n";

    return 0;
}