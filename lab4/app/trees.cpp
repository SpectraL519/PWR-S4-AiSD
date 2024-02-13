#include <iostream>
#include <fstream>
#include <algorithm>
#include <optional>
#include <random>
#include <chrono>

#include <argparse.hpp>
#include <tree/tree.hpp>
#include <tree/bst.hpp>
#include <tree/rb.hpp>
#include <tree/splay.hpp>



namespace {
    tree::itree* tree_ = nullptr;

    std::size_t N;
    std::vector <int> nodes;
    bool debug = false;

    std::string generate;
    std::string out_file_name;

    std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count()); // mersenne twister
    std::uniform_int_distribution <int> uniform;



    void parse_args (const int argc, char* argv[]) {
        argparse::ArgumentParser parser("trees");
        parser
            .add_argument("-t", "--tree")
            .default_value("bst")
            .action([] (const std::string& value) {
                static const std::vector <std::string> choices = { "bst", "rb", "splay" };
                if (std::find(choices.begin(), choices.end(), value) != choices.end())
                    return value;
                return std::string("bst");
            })
            .help("tree type - can be one of: bst, rb, splay");

        parser
            .add_argument("-n", "--number")
            .scan<'i', int>()
            .help("the number of nodes to insert to the tree");

        parser
            .add_argument("-g", "--generate")
            .default_value("random")
            .action([] (const std::string& value) {
                static const std::vector <std::string> choices = { "random", "ascending", "descending" };
                if (std::find(choices.begin(), choices.end(), value) != choices.end())
                    return value;
                return std::string("random");
            })
            .help("the way of generating the input nodes - can be one of: random, ascending, descending");

        try {
            parser.parse_args(argc, argv);
        }
        catch (const std::runtime_error& err) {
            std::cerr << err.what() << std::endl;
            std::cerr << parser;
            std::exit(1);
        }

        try {
            std::string tree_type = parser.get("tree");
            if (tree_type == "rb")
                tree_ = new tree::rb();
            else if (tree_type == "splay")
                tree_ = new tree::splay();
            else
                tree_ = new tree::bst();

            if (!parser.present<int>("number"))
                throw std::logic_error("Error: missing argument - number");

            N = parser.get<int>("number");
            nodes.resize(N);
            debug = (N <= 50);

            generate = parser.get("generate");
            if (generate == "ascending")
                std::iota(nodes.begin(), nodes.end(), 0);
            else if (generate == "descending")
                std::iota(std::rbegin(nodes), std::rend(nodes), 0);
            else {
                uniform.param(std::uniform_int_distribution<int>::param_type(0, (2 * N) - 1));
                std::generate(
                    nodes.begin(), 
                    nodes.end(), 
                    [] () { return uniform(generator); }
                );
            }

            out_file_name = "results/" + tree_type + ".csv";
        }
        catch (const std::logic_error& err) {
            std::cerr << err.what() << std::endl;
            std::exit(1);
        }
    }
}



int main (int argc, char* argv[]) {
    parse_args(argc, argv);

    tree::complexity_t ct_avg = { 0, 0, 0 };
    tree::complexity_t ct_worst = { 0, 0, 0 };

    // insert all nodes
    for (const int& key : nodes) {
        if (debug)
            std::cout << "insert " << key << std::endl;

        tree::complexity_t ct = tree_->insert_key(key);
        ct_avg += ct;
        ct_worst = tree::complexity_t::worst(ct_worst, ct);

        if (debug) {
            tree_->display();
            std::cout << std::endl;
        }

    }

    // delete all nodes randomly
    std::size_t idx, key;
    while (!tree_->empty() && !nodes.empty()) {
        idx = uniform(generator) % nodes.size();
        key = nodes[idx];

        if (debug)
            std::cout << "delete " << key << std::endl;

        tree::complexity_t ct = tree_->delete_key(key);
        ct_avg += ct;
        ct_worst = tree::complexity_t::worst(ct_worst, ct);

        if (debug) {
            tree_->display();
            std::cout << std::endl;
        }

        nodes.erase(nodes.begin() + idx);
    }

    std::cout << std::endl;
    std::cout << "avg case complexity: " << std::endl << ct_avg << std::endl;
    std::cout << "worst case complexity: " << std::endl << ct_worst << std::endl;

    try {
        std::ofstream out_file;
        out_file.open(out_file_name, std::ios::app);

        out_file << N << "," 
                 << generate << ","
                 << "avg,"
                 << ct_avg.comparisons / (float)(2 * N) << "," 
                 << ct_avg.ptr_ops / (float)(2 * N) << "," 
                 << ct_avg.height / (float)(2 * N) << "\n";

        out_file << N << "," 
                 << generate << ","
                 << "worst,"
                 << ct_worst.comparisons << "," 
                 << ct_worst.ptr_ops << "," 
                 << ct_worst.height << "\n";
        
        out_file.close();
    }
    catch (std::exception& e) {
        std::cout << "Error: Could not read '" << out_file_name << "'!\n\t";
        std::cout << e.what() << "\n";
        std::exit(1);
    }

    return 0;
}