#include "tokenizer.h"
#include <iostream>
#include <chrono>
#include <string>
#include <unordered_set>
#include <vector>

int main() {
    std::unordered_set<std::string> dict = {
        "你好", "世界", "中国", "人民", "共和国", "计算机", "科学", "技术",
        "人工", "智能", "机器学习", "深度学习", "自然语言", "处理"
    };
    
    Tokenizer tokenizer(dict);
    
    std::string text;
    std::vector<std::string> words = {"你好", "世界", "中国", "人民", "计算机", "科学", "技术", "人工", "智能"};
    for (int i = 0; i < 10000; ++i) {
        for (const auto& w : words) {
            text += w;
        }
    }
    
    std::cout << "测试文本大小: " << text.size() << " 字节" << std::endl;
    std::cout << "词典大小: " << dict.size() << " 个词" << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    auto tokens = tokenizer.tokenize(text);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "分词结果: " << tokens.size() << " 个词" << std::endl;
    std::cout << "耗时: " << duration.count() << " ms" << std::endl;
    
    double mb = text.size() / 1024.0 / 1024.0;
    double seconds = duration.count() / 1000.0;
    if (seconds > 0) {
        std::cout << "吞吐量: " << (mb / seconds) << " MB/s" << std::endl;
    }
    
    return 0;
}
