#include "thread_pool.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

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
    // 生成测试文本（10万字）
    std::string text;
    for (int i = 0; i < 10000000; ++i) {
        text += "word ";
    }
    
    std::cout << "测试文本大小: " << text.size() << " 字节" << std::endl;
    
    // 单线程
    auto start = std::chrono::high_resolution_clock::now();
    int single_result = count_words(text);
    auto end = std::chrono::high_resolution_clock::now();
    auto single_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "单线程: " << single_result << " 个词, 耗时 " << single_time.count() << " ms" << std::endl;
    
    // 多线程 (4 线程)
    ThreadPool pool(4);
    std::vector<std::future<int>> futures;
    int chunk_size = text.size() / 4;
    
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 4; ++i) {
        int start_pos = i * chunk_size;
        int end_pos = (i == 3) ? text.size() : (i + 1) * chunk_size;
        futures.push_back(pool.submit([&text, start_pos, end_pos]() {
            return count_words(text.substr(start_pos, end_pos - start_pos));
        }));
    }
    
    int multi_result = 0;
    for (auto& f : futures) {
        multi_result += f.get();
    }
    end = std::chrono::high_resolution_clock::now();
    auto multi_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "4 线程: " << multi_result << " 个词, 耗时 " << multi_time.count() << " ms" << std::endl;
    std::cout << "加速比: " << std::fixed << std::setprecision(2) 
              << (double)single_time.count() / multi_time.count() << "x" << std::endl;
    
    return 0;
}
