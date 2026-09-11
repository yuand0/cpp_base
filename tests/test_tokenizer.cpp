#include <gtest/gtest.h>
#include "tokenizer.h"

TEST(TokenizerTest, EmptyDict) {
    Tokenizer t;
    auto tokens = t.tokenize("你好世界");
    EXPECT_EQ(tokens.size(), 4);  // 无词典时按单字切分
}

TEST(TokenizerTest, MaxMatching) {
    std::unordered_set<std::string> dict = {"你好", "世界", "你好世界"};
    Tokenizer t(dict);
    auto tokens = t.tokenize("你好世界");
    EXPECT_EQ(tokens.size(), 1);  // 最大匹配优先匹配 "你好世界"
    EXPECT_EQ(tokens[0], "你好世界");
}

TEST(TokenizerTest, EnglishWords) {
    Tokenizer t;
    auto tokens = t.tokenize("hello world");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "hello");
    EXPECT_EQ(tokens[1], "world");
}

TEST(TokenizerTest, MixedText) {
    std::unordered_set<std::string> dict = {"你好", "世界"};
    Tokenizer t(dict);
    auto tokens = t.tokenize("你好 world 世界");
    EXPECT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "你好");
    EXPECT_EQ(tokens[1], "world");
    EXPECT_EQ(tokens[2], "世界");
}
