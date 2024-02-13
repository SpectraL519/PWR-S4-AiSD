#pragma once

#include <iostream>
#include <concepts>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <deque>

std::mt19937 generator;
std::uniform_int_distribution <int> uniform; 





template <typename T>
void show (std::vector <T> V) {
    for (T v : V)
        std::cout << v << " ";
    std::cout << "\n";
};


template <typename T>
    concept comparable = 
        requires (T a, T b) {
            {a == b} -> std::convertible_to<bool>;
            {a < b} -> std::convertible_to<bool>;
            {a > b} -> std::convertible_to<bool>;
    };


namespace sort {
    // `_` prefixed members should be considered private
    struct sort_t {
        int comparisons;
        int swaps;

        void operator += (sort_t st) {
            comparisons += st.comparisons;
            swaps += st.swaps;
        }
    };

    // exercise 1: insertion sort
    template <comparable T>
    sort_t insert_sort (std::vector <T> &array, bool debug);

    
    // exercise 1: merge sort
    template <comparable T>
    std::vector <T> _merge (std::vector <T> a, std::vector <T> b, sort_t &st);

    template <comparable T>
    std::vector <T> _merge_sort_unit (std::vector <T> array, sort_t &st, bool debug);
    
    template <comparable T>
    sort_t merge_sort (std::vector <T> &array, bool debug);


    // exercise 1: quick sort
    template <comparable T>
    int _partition (std::vector <T> &array, int p, int q, sort_t &st);

    template <comparable T>
    void _quick_sort_unit (std::vector <T> &array, int p, int q, sort_t &st, bool debug);

    template <comparable T>
    sort_t quick_sort (std::vector <T> &array, bool debug);


    // exercise 3: dual pivot quick sort
    template <comparable T>
    std::pair <int, int> _dp_partition (std::vector <T> &array, int p, int q, sort_t &st);

    template <comparable T>
    void _dp_quick_sort_unit (std::vector <T> &array, int p, int q, sort_t &st, bool debug);
    
    template <comparable T>
    sort_t dual_pivot_quick_sort (std::vector <T> &array, bool debug);


    // exercise 4: hybrid sort
    template <comparable T>
    void _tim_insert (std::vector <T> &array, int first, int last, sort_t &st);

    template <comparable T>
    void _tim_merge (std::vector <T> &array, int first, int mid, int last, sort_t &st);

    template <comparable T>
    sort_t tim_sort (std::vector <T> &array, bool debug);
};



template <comparable T>
sort::sort_t sort::insert_sort (std::vector <T> &array, bool debug) {
    int cmp = 0;
    int swp = 0;

    int size = array.size();
    for (int j = 1; j < size; j++) {
        T key = array[j];
        int i = j - 1;

        cmp++;
        while (i >= 0 && array[i] > key) {
            array[i + 1] = array[i];
            i--;
            cmp++;
            swp++;
        }

        array[i + 1] = key;
        swp++;

        if (debug)
            show(array);
    }

    return sort::sort_t {
        .comparisons = cmp,
        .swaps = swp,
    };
}



template <comparable T>
std::vector <T> sort::_merge (std::vector <T> a, std::vector <T> b, sort::sort_t &st) {
    std::vector <T> merged;

    typename std::vector<T>::iterator ia = a.begin();
    typename std::vector<T>::const_iterator a_end = a.end();
    typename std::vector<T>::iterator ib = b.begin();
    typename std::vector<T>::const_iterator b_end = b.end();

    while (ia != a_end && ib != b_end) {
        st.comparisons++;
        st.swaps++;
        
        if (*ia < *ib) 
            merged.push_back(*ia++); 
        else 
            merged.push_back(*ib++);
    }

    st.swaps += (int)(a_end - ia);
    while (ia != a_end)
        merged.push_back(*ia++);

    st.swaps += (int)(b_end - ib);
    while (ib != b_end)
        merged.push_back(*ib++);
        
    return merged;
}

template <comparable T>
std::vector <T> sort::_merge_sort_unit (std::vector <T> array, sort::sort_t &st, bool debug) {
    unsigned size = array.size();
    if (size < 2)      
        return array;

    typename std::vector<T>::iterator r = array.begin() + (size / 2);
    std::vector <T> a = sort::_merge_sort_unit(std::vector<T>(array.begin(), r), st, debug);
    std::vector <T> b = sort::_merge_sort_unit(std::vector<T>(r, array.end()), st, debug);

    std::vector <T> merged = _merge(a, b, st);
    if (debug) 
        show(array);

    return merged;
}

template <comparable T>
sort::sort_t sort::merge_sort (std::vector <T> &array, bool debug) {
    sort::sort_t st {
        .comparisons = 0,
        .swaps = 0,
    };

    if (debug)
        std::cout << "Merge operations:\n";

    array = sort::_merge_sort_unit(array, st, debug);
    return st;
}



template <comparable T>
int sort::_partition (std::vector <T> &array, int p, int q, sort::sort_t &st) {
    unsigned pivot_idx = uniform(generator) % (q - p) + p;
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
void sort::_quick_sort_unit (std::vector <T> &array, int p, int q, sort::sort_t &st, bool debug) {
    if (p < q) {
        int r = sort::_partition(array, p, q, st);
        if (debug) {
            std::cout << "(" << array[r] << ") ";
            show(array);
        }

        sort::_quick_sort_unit(array, p, r, st, debug);
        sort::_quick_sort_unit(array, r + 1, q, st, debug);
    }
}

template <comparable T>
sort::sort_t sort::quick_sort (std::vector <T> &array, bool debug) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);

    sort::sort_t st {
        .comparisons = 0,
        .swaps = 0,
    };

    if (debug)
        std::cout << "Partition operations:\n";

    sort::_quick_sort_unit(array, 0, array.size() - 1, st, debug);
    return st;
}



template <comparable T>
std::pair <int, int> sort::_dp_partition (std::vector <T> &array, int p, int q, sort::sort_t &st) {
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
void sort::_dp_quick_sort_unit (std::vector <T> &array, int p, int q, sort::sort_t &st, bool debug) {
    if (p < q) {
        auto[r, s] = sort::_dp_partition(array, p, q, st);
        if (debug) {
            std::cout << "(" << array[r] << ", " << array[s] << ") ";
            show(array);
        }

        sort::_dp_quick_sort_unit(array, p, r, st, debug);
        sort::_dp_quick_sort_unit(array, r + 1, s, st, debug);
        sort::_dp_quick_sort_unit(array, s + 1, q, st, debug);
    }
}

template <comparable T>
sort::sort_t sort::dual_pivot_quick_sort (std::vector <T> &array, bool debug) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);

    sort::sort_t st {
        .comparisons = 0,
        .swaps = 0,
    };

    if (debug)
        std::cout << "Partition operations:\n";

    sort::_dp_quick_sort_unit(array, 0, array.size() - 1, st, debug);
    return st;
}



template <comparable T>
void sort::_tim_insert (std::vector <T> &array, int first, int last, sort::sort_t &st) {
    for (int j = first + 1; j <= last; j++) {
        T key = array[j];
        int i = j - 1;

        st.comparisons++;
        while (i >= first && array[i] > key) {
            array[i + 1] = array[i];
            i--;
            st.comparisons++;
            st.swaps++;
        }

        array[i + 1] = key;
        st.swaps++;
    }
}

template <comparable T>
void sort::_tim_merge (std::vector <T> &array, int first, int mid, int last, sort::sort_t &st) {
    typename std::vector<T>::iterator it = array.begin();
    std::vector <T> left(it + first, it + mid + 1);
    std::vector <T> right(it + mid + 1, it + last + 1);

    typename std::vector<T>::iterator left_it = left.begin();
    typename std::vector<T>::const_iterator left_end = left.end();
    typename std::vector<T>::iterator right_it = right.begin();
    typename std::vector<T>::const_iterator right_end = right.end();

    it = array.begin() + first;
    while (left_it != left_end && right_it != right_end) {
        st.comparisons++;
        st.swaps++;

        if (*left_it < *right_it) 
            *it++ = *left_it++;
        else 
            *it++ = *right_it++;
    }
    
    st.swaps += (int)(left_end - left_it);
    while (left_it != left_end) 
        *it++ = *left_it++;

    st.swaps += (int)(right_end - right_it);
    while (right_it != right_end) 
        *it++ = *right_it++;
}

template <comparable T>
sort::sort_t sort::tim_sort (std::vector <T> &array, bool debug) {
    sort::sort_t st {
        .comparisons = 0,
        .swaps = 0,
    };

    int arr_size = array.size();
    const int RUN = 4;
    std::deque <std::vector <T>> subarrays;

    for (int i = 0; i < arr_size; i += RUN) {
        sort::_tim_insert(array, i, std::min(i + RUN - 1, arr_size - 1), st);
        if (debug) {
            std::cout << "(i): ";
            show(array);
        }
    }

    for (int size = RUN; size < arr_size; size *= 2) {
        for (int first = 0; first < arr_size; first += 2 * size) {
            int mid = first + size - 1;
            int last = std::min((first + 2 * size - 1), arr_size - 1);

            if (mid < last) {
                sort::_tim_merge(array, first, mid, last, st);
                if (debug) {
                    std::cout << "(m): ";
                    show(array);
                }
            }
        }
    }

    return st;
}