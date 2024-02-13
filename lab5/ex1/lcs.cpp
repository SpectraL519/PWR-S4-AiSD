#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <vector>
#include <optional>



namespace {
    std::string random_str (const  std::size_t length) {
        static const char charset[] = "abcdefghijklmnopqrstuvwxyz";
        static std::mt19937 mt(std::chrono::system_clock::now().time_since_epoch().count()); // mersenne twister
        static std::uniform_int_distribution <std::size_t> uniform(0, sizeof(charset) - 1);

        std::string str(length, 0);
        std::generate_n(str.begin(), length, [] () { return charset[uniform(mt)]; } );
        return std::move(str);
    }

    std::optional<std::string> lcs (const std::string &a, const std::string &b) {
        std::size_t a_len = a.length() + 1;
        std::size_t b_len = b.length() + 1;

        std::vector <std::vector <std::size_t>> len(a_len, std::vector<std::size_t>(b_len, 0));

        for (std::size_t i = 0; i < a_len; i++) {
            for (std::size_t j = 0; j < b_len; j++) {
                if (i == 0 || j == 0) 
                    len[i][j] = 0;
                else 
                    if (a[i - 1] == b[j - 1])
                        len[i][j] = len[i - 1][j - 1] + 1;
                    else
                        len[i][j] = std::max(len[i - 1][j], len[i][j - 1]);
            }
        }

        std::size_t idx = len[a.size()][b.size()];

        if (idx == 0)
            return std::nullopt;

        char lcs[idx + 1];
        lcs[idx] = '\0';

        std::size_t i = a.size(), j = b.size();
        while (i > 0 && j > 0) {
            if (a[i - 1] == b[j - 1]) {
                lcs[idx - 1] = a[i - 1];
                i--;
                j--;
                idx--;
            }
            else {
                if (len[i - 1][j] > len[i][j - 1])
                    i--;
                else
                    j--;
            }
        }

        return std::string(lcs);
    }

    std::size_t lcs_length (const std::string a, const std::string b) {
        std::size_t a_len = a.length() + 1;
        std::size_t b_len = b.length() + 1;

        std::vector <std::vector <std::size_t>> len(a_len, std::vector<std::size_t>(b_len, 0));

        for (std::size_t i = 0; i < a_len; i++) {
            for (std::size_t j = 0; j < b_len; j++) {
                if (i == 0 || j == 0) 
                    len[i][j] = 0;
                else 
                    if (a[i - 1] == b[j - 1])
                        len[i][j] = len[i - 1][j - 1] + 1;
                    else
                        len[i][j] = std::max(len[i - 1][j], len[i][j - 1]);
            }
        }

        return len[a.length()][b.length()];
   }
}



int main (int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Invalid arguments!" << std::endl;
        return 1;
    }

    std::size_t length;
    std::string length_str = argv[1];

    try {
        std::size_t pos;
        length = std::stoi(argv[1], &pos);
        if (pos < length_str.size()) {
            std::cerr << "Error: Trailing characters after number: " << length_str << '\n';
            std::exit(1);
        }
    } 
    catch (std::invalid_argument const &ex) {
        std::cerr << "Error: Invalid number: " << length_str << '\n';
        std::exit(1);
    } 
    catch (std::out_of_range const &ex) {
        std::cerr << "Error: Number out of range: " << length_str << '\n';
        std::exit(1);
    }

    const std::string a = random_str(length);
    const std::string b = random_str(length);

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // std::size_t lcs_len = lcs_length(a, b);
    std::optional<std::string> lcs_ = lcs(a, b);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    auto exec_time = (float)duration.count() / 1000;

    if (lcs_) {
        std::cout << "lcs_len = " << lcs_.value().length() << std::endl;
        std::cout << "lcs = " << lcs_.value() << std::endl;
    }
    else {
        std::cout << "lcs = !" << std::endl;
    }

    std::cout << "execution time = " << exec_time  << std::endl;

    std::ofstream out_file("lcs.csv", std::ios_base::app);
    if (!out_file) {
        std::cerr << "Error: Cannot open 'lcs.csv'!" << std::endl;
        return 1;
    }

    out_file << length << "," << exec_time << std::endl;

    return 0;
}