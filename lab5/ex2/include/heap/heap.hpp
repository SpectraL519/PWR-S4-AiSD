#pragma once

#include <iostream>
#include <optional>

#include <comparable.hpp>



namespace heap {
    template <comparable::weakly_ordered T>
    class i_heap {
        public:
            virtual std::size_t size() = 0;
            virtual bool empty() = 0;

            virtual void insert_key (const T key) = 0;
            virtual std::optional<T> min_key() = 0;
            virtual std::optional<T> extract_min() = 0;

            virtual void merge (const i_heap<T>& h_prim) = 0; // heap union

            virtual void show() = 0;
    };
} // namespace heap