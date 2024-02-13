#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

#include <heap/binomial_heap.hpp>
#include <heap/fibonacci_heap.hpp>



int main (int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Error: Invalid arguments\n";
        std::cerr << "\tScript invocation syntax: `./heap <type> <n>`";
        return 1;
    }

    // init heaps:  h and h'
    std::string heap_type = argv[1];
    heap::i_heap<int>* h = nullptr;
    heap::i_heap<int>* h_prim = nullptr;

    if (heap_type == "binomial") {
        h = new heap::binomial_heap<int>();
        h_prim = new heap::binomial_heap<int>();
    }
    else if (heap_type == "fibonacci") {
        h = new heap::fibonacci_heap<int>();
        h_prim = new heap::fibonacci_heap<int>();
    }
    else {
        std::cerr << "Error: Invalid value of <type> - must be 'binomial' or 'fibonacci'\n";
        return 1;
    }

    std::size_t n;
    std::string n_str = argv[2];

    try {
        std::size_t pos;
        n = std::stoi(argv[2], &pos);
        if (pos < n_str.size()) {
            std::cerr << "Error: Trailing characters after number: " << n_str << '\n';
            std::exit(1);
        }
    } 
    catch (std::invalid_argument const &ex) {
        std::cerr << "Error: Invalid number: " << n_str << '\n';
        std::exit(1);
    } 
    catch (std::out_of_range const &ex) {
        std::cerr << "Error: Number out of range: " << n_str << '\n';
        std::exit(1);
    }

    bool debug = (n <= 20);
    std::vector<int> nodes(n);

    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count()); // mersenne twister
    std::uniform_int_distribution <int> uniform;
    uniform.param(std::uniform_int_distribution<int>::param_type(0, (2 * n) - 1));

    // insert nodes into h
    std::generate(
        nodes.begin(), 
        nodes.end(), 
        [&generator, &uniform] () { return uniform(generator); }
    );

    std::cout << std::endl << "bulding heap H1" << std::endl;
    for (const int& key : nodes) {
        if (debug)
            std::cout << "insert " << key << std::endl;
        h->insert_key(key);
    }

    // insert nodes into h'
    std::generate(
        nodes.begin(), 
        nodes.end(), 
        [&generator, &uniform] () { return uniform(generator); }
    );

    std::cout << std::endl << "bulding heap H2" << std::endl;
    for (const int& key : nodes) {
        if (debug)
            std::cout << "insert " << key << std::endl;
        h_prim->insert_key(key);
    }

    std::cout <<  std::endl << "heap union\n" << std::endl;
    h->merge(*h_prim);
    if (debug) {
        h->show();
        std::cout << std::endl;
    }

    int min = -1;
    while (!h->empty()) {
        int min_ = h->extract_min().value();
        if (debug) {
            std::cout << "extracted " << min_ << std::endl;
            if (heap_type == "fibonacci") {
                h->show();
                std::cout << std::endl;
            }
        }

        if (min_ < min) {
            std::cerr << "Error: Keys extracted in a not nondecreasing order!\n";
            return 1;
        }

        min = min_;
    }

    if (debug) {
        std::cout << std::endl;
        h->show();
    }

    return 0;
}