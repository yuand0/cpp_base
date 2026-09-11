#include "tokenizer.h"
#include <cctype>

Tokenizer::Tokenizer() {}
Tokenizer::Tokenizer(const std::unordered_set<std::string>& dict) : dict_(dict) {}

void Tokenizer::load_dict(const std::unordered_set<std::string>& dict) {
    dict_ = dict;
}

bool Tokenizer::is_chinese(char c) const {
    return (static_cast<unsigned char>(c) & 0x80) != 0;
}

size_t Tokenizer::utf8_char_len(unsigned char c) const {
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

std::vector<std::string> Tokenizer::tokenize(const std::string& text) const {
    std::vector<std::string> tokens;
    size_t i = 0;
    size_t n = text.size();

    while (i < n) {
        // 跳过空格和标点
        if (std::isspace(static_cast<unsigned char>(text[i]))) {
            ++i;
            continue;
        }

        // 非中文（英文/数字）：连续读取直到遇到非字母数字
        if (!is_chinese(text[i])) {
            size_t start = i;
            while (i < n && !is_chinese(text[i]) && !std::isspace(static_cast<unsigned char>(text[i]))) {
                ++i;
            }
            tokens.push_back(text.substr(start, i - start));
            continue;
        }

        // 中文：最大匹配（从长到短）
        bool matched = false;
        size_t max_len = 4;  // 最多匹配 4 个汉字
        for (size_t len = max_len; len >= 1; --len) {
            size_t byte_len = 0;
            size_t char_count = 0;
            size_t j = i;
            while (j < n && char_count < len) {
                unsigned char c = static_cast<unsigned char>(text[j]);
                byte_len += utf8_char_len(c);
                j += utf8_char_len(c);
                ++char_count;
            }
            if (char_count < len) continue;

            std::string word = text.substr(i, byte_len);
            if (dict_.find(word) != dict_.end()) {
                tokens.push_back(word);
                i += byte_len;
                matched = true;
                break;
            }
        }

        // 未匹配到词典中的词，按单字切分
        if (!matched) {
            unsigned char c = static_cast<unsigned char>(text[i]);
            size_t len = utf8_char_len(c);
            tokens.push_back(text.substr(i, len));
            i += len;
        }
    }

    return tokens;
}
