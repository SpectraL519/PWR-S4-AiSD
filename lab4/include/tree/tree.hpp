#pragma once

#include <cstddef>
#include <iostream>



namespace tree {
    struct complexity_t {
        std::size_t comparisons;
        std::size_t ptr_ops;
        std::size_t height;

        complexity_t& operator += (const complexity_t& other) {
            comparisons += other.comparisons;
            ptr_ops += other.ptr_ops;
            height += other.height;
            return *this;
        }

        static complexity_t worst (const complexity_t& lhs, const complexity_t& rhs) {
            return std::move(complexity_t {
                .comparisons = std::max(lhs.comparisons, rhs.comparisons),
                .ptr_ops = std::max(lhs.ptr_ops, rhs.ptr_ops),
                .height = std::max(lhs.height, rhs.height)
            });
        }

        friend std::ostream& operator << (std::ostream& os, const complexity_t& ct) {
            os << "\t.comparisons = " << ct.comparisons << std::endl
               << "\t.pointer_operations = " << ct.ptr_ops << std::endl
               << "\t.height = " << ct.height << std::endl;
            return os;     
        }
    };

    class itree {
        public:
            virtual const std::size_t size() = 0;
            virtual const bool empty() = 0;
            virtual const std::size_t height() = 0;
            
            virtual const bool search (const int key) = 0;
            virtual const complexity_t insert_key (const int key) = 0;
            virtual const complexity_t delete_key (const int key) = 0;

            virtual void display() = 0;
    };
}