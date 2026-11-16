#include "calculator.h"
#include <iostream>
#include <chrono>

int main() {
    Calculator c;
    const std::string expr = "3 + 5 * (2 - 8) / 2 - (-4)";
    const int iterations = 100000;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        volatile double r = c.evaluate(expr);
        (void)r;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "表达式: " << expr << std::endl;
    std::cout << "迭代次数: " << iterations << std::endl;
    std::cout << "总耗时: " << duration.count() << " μs" << std::endl;
    std::cout << "单次耗时: " << (double)duration.count() / iterations << " μs" << std::endl;
    
    return 0;
}
