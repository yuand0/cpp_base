#include "thread_pool.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

int count_words(const std::string& text) {
    std::istringstream stream(text);
    std::string word;
    int count = 0;
    while (stream >> word) {
        ++count;
    }
    return count;
}

int main() {
    // 生成 5000 万词测试文本
    std::string text;
    for (int i = 0; i < 10000000; ++i) {
        text += "word ";
    }
    
    std::cout << "测试文本大小: " << text.size() << " 字节" << std::endl;
    std::cout << "总词数: 10,000,000" << std::endl;
    std::cout << std::endl;
    
    // 单线程
    auto start = std::chrono::high_resolution_clock::now();
    int single_result = count_words(text);
    auto end = std::chrono::high_resolution_clock::now();
    auto single_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "1 线程: " << single_result << " 个词, 耗时 " << single_time.count() << " ms" << std::endl;
    
    // 2 线程
    ThreadPool pool2(2);
    std::vector<std::future<int>> futures2;
    int chunk_size2 = text.size() / 2;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 2; ++i) {
        int start_pos = i * chunk_size2;
        int end_pos = (i == 1) ? text.size() : (i + 1) * chunk_size2;
        futures2.push_back(pool2.submit([&text, start_pos, end_pos]() {
            return count_words(text.substr(start_pos, end_pos - start_pos));
        }));
    }
    int multi_result2 = 0;
    for (auto& f : futures2) {
        multi_result2 += f.get();
    }
    end = std::chrono::high_resolution_clock::now();
    auto multi_time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "2 线程: " << multi_result2 << " 个词, 耗时 " << multi_time2.count() << " ms, 加速比: " 
              << std::fixed << std::setprecision(2) << (double)single_time.count() / multi_time2.count() << "x" << std::endl;
    
    // 4 线程
    ThreadPool pool4(4);
    std::vector<std::future<int>> futures4;
    int chunk_size4 = text.size() / 4;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 4; ++i) {
        int start_pos = i * chunk_size4;
        int end_pos = (i == 3) ? text.size() : (i + 1) * chunk_size4;
        futures4.push_back(pool4.submit([&text, start_pos, end_pos]() {
            return count_words(text.substr(start_pos, end_pos - start_pos));
        }));
    }
    int multi_result4 = 0;
    for (auto& f : futures4) {
        multi_result4 += f.get();
    }
    end = std::chrono::high_resolution_clock::now();
    auto multi_time4 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "4 线程: " << multi_result4 << " 个词, 耗时 " << multi_time4.count() << " ms, 加速比: " 
              << std::fixed << std::setprecision(2) << (double)single_time.count() / multi_time4.count() << "x" << std::endl;
    
    return 0;
}
