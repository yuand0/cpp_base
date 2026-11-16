#pragma once
#include <string>
#include <vector>
#include <unordered_set>

class Tokenizer {
public:
    Tokenizer();
    explicit Tokenizer(const std::unordered_set<std::string>& dict);
    void load_dict(const std::unordered_set<std::string>& dict);
    std::vector<std::string> tokenize(const std::string& text) const;

private:
    std::unordered_set<std::string> dict_;
    bool is_chinese(char c) const;
    size_t utf8_char_len(unsigned char c) const;  // ← 这一行必须有
};
