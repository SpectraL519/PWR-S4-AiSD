#pragma once

#include <iostream>
#include <vector>
#include <concepts>
#include <cassert>
#include <random>
#include <chrono>

std::mt19937 generator;
std::uniform_int_distribution <int> uniform; 





template <typename T>
void show (std::vector <T> V, int p, int q) {
    for (int i = p; i <= q; i++)
        std::cout << V[i] << " ";
    std::cout << "\n";
};



template <typename T>
    concept comparable = 
        requires (T a, T b) {
            {a == b} -> std::convertible_to<bool>;
            {a < b} -> std::convertible_to<bool>;
            {a > b} -> std::convertible_to<bool>;
    };



namespace select {
    // `_` prefixed members should be considered private

    static std::size_t subarray_size = 5;

    struct select_t {
        unsigned comparisons;
        unsigned swaps;

        void operator += (const select_t &st) {
            comparisons += st.comparisons;
            swaps += st.swaps;
        }
    };


    template <comparable T>
    int _set_pivot_partition (std::vector <T> &array, int p, int q, T pivot, select_t &st);

    template <comparable T>
    T _random_select_unit (std::vector <T> &array, int p, int q, std::size_t k, select_t &st, bool debug);

    template <comparable T>
    std::pair <T, select_t> random_select (std::vector <T> array, std::size_t k, bool debug);


    template <comparable T>
    void _insert_sort (T *array, std::size_t size, bool debug, select_t &st);
    
    template <comparable T>
    T _median_select_unit (std::vector <T> &array, int p, int q, std::size_t k, select_t &st, bool debug);

    template <comparable T>
    std::pair <T, select_t> median_select (std::vector <T> array, std::size_t k, bool debug);
}



template <comparable T>
int select::_set_pivot_partition (std::vector <T> &array, int p, int q, T pivot, select_t &st) {
    int i = p - 1, j = q + 1;
    while (true) {
        while (++st.comparisons && array[++i] < pivot) {}
        while (++st.comparisons && array[--j] > pivot) {}

        if (i >= j) 
            return j;

        std::swap(array[i], array[j]);
        st.swaps++;
    }
}

template <comparable T>
T select::_random_select_unit (std::vector <T> &array, int p, int q, std::size_t k, select_t &st, bool debug) {
    if (debug) {
        std::cout << "array: ";
        show(array, p, q);
    }
    
    if (p == q) 
        return array[p];

    uniform.param(std::uniform_int_distribution<int>::param_type(p, q));
    int r = _set_pivot_partition(array, p, q, array[uniform(generator)], st);
    if (debug) {
        std::cout << "after partition: ";
        show(array, p, q);
        std::cout << "pivot = " << array[r] << " (index: " << r << ")\n\n";
    }

    if (r == k) 
        return array[r];
    if (r < k) 
        return _random_select_unit(array, r + 1, q, k, st, debug);
    return _random_select_unit(array, p, r, k, st, debug);
}

template <comparable T>
std::pair <T, select::select_t> select::random_select (std::vector <T> array, std::size_t k, bool debug) {
    assert(k > 0 && k <= array.size());
    k -= 1;
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
    select_t st {
        .comparisons = 0,
        .swaps = 0,
    };

    if (debug)
        std::cout << "Intermediate states:\n";

    return std::make_pair(_random_select_unit(array, 0, array.size() - 1, k, st, debug), st);
}



template <comparable T>
void select::_insert_sort (T *array, std::size_t size, bool debug, select_t &st) {
    for (int j = 1; j < size; j++) {
        T key = array[j];
        int i = j - 1;

        while (++st.comparisons && i >= 0 && array[i] > key) {
            array[i + 1] = array[i];
            i--;
            st.swaps++;
        }

        array[i + 1] = key;
        st.swaps++;
    }

    if (debug) {
        std::cout << "insert_sort: ";
        show(std::vector<T>(array, array + size), 0, size - 1);
    }
}

template <comparable T>
T select::_median_select_unit (std::vector <T> &array, int p, int q, std::size_t k, 
        select_t &st, bool debug) {
    if (debug) {
        std::cout << "array: ";
        show(array, p, q);
    }
    
    std::size_t size = q - p + 1;
    if (size <= subarray_size) {
        _insert_sort(&(*(array.begin() + p)), size, debug, st);
        return array[k];
    }

    std::size_t num_subarrays = (q - p + subarray_size) / subarray_size;
    std::vector <T> medians(num_subarrays);

    std::size_t m_idx = 0;
    for (std::size_t i = p; i <= q; i += subarray_size) {
        std::size_t tmp_size = std::min(subarray_size, q - i + 1);
        _insert_sort(&(*(array.begin() + i)), tmp_size, debug, st);
        medians[m_idx++] = array[i + (tmp_size / 2)];
    }

    if (debug)
        std::cout << "selecting the median of medians:\n\t";
    T median = _median_select_unit(medians, 0, medians.size() - 1, medians.size() / 2, st, debug);
    int r = _set_pivot_partition(array, p, q, median, st);
    if (debug) {
        std::cout << "array (after parition): ";
        show(array, p, q);
        std::cout << "median (pivot) index: " << r << "\n\n";
    }

    if (r == k)
        return median;
    if (r > k) 
        return _median_select_unit(array, p, r - 1, k, st, debug);
    return _median_select_unit(array, r + 1, q, k, st, debug);
}

template <comparable T>
std::pair <T, select::select_t> select::median_select (std::vector <T> array, std::size_t k, bool debug) {
    assert(k > 0 && k <= array.size());
    k -= 1;
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
    select_t st {
        .comparisons = 0,
        .swaps = 0,
    };

    if (debug)
        std::cout << "Intermediate states:\n";

    return std::make_pair(
        _median_select_unit(array, 0, array.size() - 1, k, st, debug), st
        );
}
