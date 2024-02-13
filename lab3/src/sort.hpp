#pragma once

#include <iostream>
#include <concepts>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <deque>
#include "select.hpp"





namespace sort {
    // `_` prefixed members should be considered private
    template <comparable T>
    int _partition (std::vector <T> &array, int p, int q, select::select_t &st, bool debug);

    template <comparable T>
    void _quick_sort_unit (std::vector <T> &array, int p, int q, select::select_t &st, bool debug, bool use_select);

    template <comparable T>
    select::select_t quick_sort (std::vector <T> &array, bool debug, bool use_select);


    template <comparable T>
    std::pair <int, int> _dp_partition (std::vector <T> &array, int p, int q, select::select_t &st);

    template <comparable T>
    void _dp_quick_sort_unit (std::vector <T> &array, int p, int q, select::select_t &st, bool debug, bool use_select);
    
    template <comparable T>
    select::select_t dual_pivot_quick_sort (std::vector <T> &array, bool debug, bool use_select);
}



template <comparable T>
int sort::_partition (std::vector <T> &array, int p, int q, select::select_t &st, bool debug) {
    std::size_t pivot_idx = uniform(generator) % (q - p) + p;
    T pivot = array[pivot_idx];
    std::swap(array[p], array[pivot_idx]);
    st.swaps++;

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
void sort::_quick_sort_unit (std::vector <T> &array, int p, int q, select::select_t &st, bool debug, bool use_select) {
    if (p < q) {
        int r;
        if (use_select) {
            r = p + ((q - p) / 2);
            select::_random_select_unit(array, p, q, r, st, debug);
        }
        else
            r = sort::_partition(array, p, q, st, debug);

        if (debug) {
            std::cout << "(" << array[r] << ") ";
            show(array, p, q);
        }

        sort::_quick_sort_unit(array, p, r, st, debug, use_select);
        sort::_quick_sort_unit(array, r + 1, q, st, debug, use_select);
    }
}

template <comparable T>
select::select_t sort::quick_sort (std::vector <T> &array, bool debug, bool use_select) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);

    select::select_t st {
        .comparisons = 0,
        .swaps = 0,
    };

    if (debug)
        std::cout << "Partition operations:\n";

    sort::_quick_sort_unit(array, 0, array.size() - 1, st, debug, use_select);
    return st;
}



template <comparable T>
std::pair <int, int> sort::_dp_partition (std::vector <T> &array, int p, int q, select::select_t &st) {
    st.comparisons++;
    if (array[p] > array[q]) {
        std::swap(array[p], array[q]);
        st.swaps++;
    }

    T pivot_low = array[p];
    T pivot_high = array[q];
    int low = p + 1, high = q - 1;
    int i = low;

    while (low <= high) {
        if (++st.comparisons && array[low] < pivot_low) {
            std::swap(array[low], array[i]);
            st.swaps++;
            i++;
        }
        else if (st.comparisons++ && array[low] >= pivot_high) {
            while (+st.comparisons && array[high] > pivot_high && low < high)
                high--;

            std::swap(array[low], array[high]);
            st.swaps++;
            high--;

            if (++st.comparisons && array[low] < pivot_low) {
                std::swap(array[low], array[i]);
                st.swaps++;
                i++;
            }
        }
        
        low++;
    }

    i--;
    high++;
 
    std::swap(array[p], array[i]);
    std::swap(array[q], array[high]);
    st.swaps += 2;
 
    return std::make_pair(i, high);
}

template <comparable T>
void sort::_dp_quick_sort_unit (std::vector <T> &array, int p, int q, select::select_t &st, bool debug, bool use_select) {
    if (p < q) {
        int r, s;
        if (use_select) {
            if (debug) 
                std::cout << "\nselecting low pivot:\n";
            r = p + ((q - p) / 3);
            select::_random_select_unit(array, p, q, r, st, debug);

            if (debug) 
                std::cout << "selecting high pivot:\n";
            s = p + ((q - p) / 3 * 2);
            select::_random_select_unit(array, p, q, s, st, debug);
        }
        else {
            auto[l, h] = sort::_dp_partition(array, p, q, st);
            r = l; s = h;
        }

        if (debug) {
            std::cout << "(" << array[r] << ", " << array[s] << ") ";
            show(array, p, q);
        }

        sort::_dp_quick_sort_unit(array, p, r, st, debug, use_select);
        sort::_dp_quick_sort_unit(array, r + 1, s, st, debug, use_select);
        sort::_dp_quick_sort_unit(array, s + 1, q, st, debug, use_select);
    }
}

template <comparable T>
select::select_t sort::dual_pivot_quick_sort (std::vector <T> &array, bool debug, bool use_select) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);

    select::select_t st {
        .comparisons = 0,
        .swaps = 0,
    };

    if (debug)
        std::cout << "Partition operations:\n";

    sort::_dp_quick_sort_unit(array, 0, array.size() - 1, st, debug, use_select);
    return st;
}
