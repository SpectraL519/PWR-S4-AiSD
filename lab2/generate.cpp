#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <random>
#include <chrono>
#include <algorithm>



int main (int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Error: Invalid arguments\n";
        std::cerr << "\tScript invocation syntax: `./generate <n> <how>`";
        return 1;
    }

    int arr_size;
    std::vector <int> array;
    std::string arr_size_s = argv[1];
    std::string how = argv[2];

    try {
        std::size_t pos;
        arr_size = std::stoi(argv[1], &pos);
        if (pos < arr_size_s.size()) {
            std::cerr << "Trailing characters after number: " << arr_size_s << '\n';
            std::exit(1);
        }
        array.resize(arr_size, 0);
    } 
    catch (std::invalid_argument const &ex) {
        std::cerr << "Invalid number: " << arr_size_s << '\n';
        std::exit(1);
    } 
    catch (std::out_of_range const &ex) {
        std::cerr << "Number out of range: " << arr_size_s << '\n';
        std::exit(1);
    }

    
    if (how == "random") {
        std::mt19937 generator; // mersenne twister
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
        std::uniform_int_distribution <int> uniform {0, (2 * arr_size) - 1};

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
    for (int a : array)
        std::cout << a << " ";
    std::cout << "\n";

    return 0;
}