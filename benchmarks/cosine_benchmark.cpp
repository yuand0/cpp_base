#include "cosine.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <random>

int main() {
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    std::vector<size_t> dims = {128, 256, 512, 1024, 2048, 4096};
    
    std::cout << "维度,耗时(us)" << std::endl;
    for (size_t dim : dims) {
        std::vector<double> a(dim), b(dim);
        for (size_t i = 0; i < dim; ++i) {
            a[i] = dist(gen);
            b[i] = dist(gen);
        }
        
        const int iterations = 1000;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            volatile double result = cosine_similarity(a, b);
            (void)result;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << dim << "," << duration.count() / iterations << std::endl;
    }
    return 0;
}
