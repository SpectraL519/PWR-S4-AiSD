#pragma once

#include <concepts>
#include <compare>



namespace comparable {
    template <typename T>
    concept weakly_ordered = 
        std::default_initializable<T> &&
        requires (T a, T b) {
            {a <=> b} -> std::convertible_to <std::weak_ordering>;

            // {a < b} -> std::convertible_to<bool>;
            // {a <= b} -> std::convertible_to<bool>;
            // {a > b} -> std::convertible_to<bool>;
            // {a >= b} -> std::convertible_to<bool>;
        };
}