#define BENCHPRESS_CONFIG_MAIN
#include "benchpress.hpp"
#include "allocator.hpp"
#include "my_vector.hpp"
#include <vector>

typedef std::vector<int, my::allocator<int>> std_vector_my_alloc;
typedef my::vector<int, my::allocator<int>> my_vector_my_alloc;

BENCHMARK("std::vector with std::allocator", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std::vector<int> a;
        for (auto j = 0; j <= 100000; ++j) {
            a.push_back(100);
        }
    }
})

BENCHMARK("std::vector with my::allocator", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        std_vector_my_alloc a;
        for (auto j = 0; j <= 100000; ++j) {
            a.push_back(100);
        }
    }
})

BENCHMARK("my::vector with std::allocator", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        my::vector<int> a;
        for (auto j = 0; j <= 100000; ++j) {
            a.push_back(100);
        }
    }
})

BENCHMARK("my::vector with my::allocator", [](benchpress::context* ctx) {
    for (size_t i = 0; i < ctx->num_iterations(); ++i) {
        my_vector_my_alloc a;
        for (auto j = 0; j <= 100000; ++j) {
            a.push_back(100);
        }
    }
})