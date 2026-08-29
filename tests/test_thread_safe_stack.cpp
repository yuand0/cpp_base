#include <gtest/gtest.h>
#include "thread_safe_stack.h"
#include <thread>
#include <vector>

TEST(ThreadSafeStackTest, PushPop) {
    ThreadSafeStack<int> stack;
    stack.push(42);
    auto val = stack.pop();
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 42);
    EXPECT_TRUE(stack.empty());
}

TEST(ThreadSafeStackTest, WaitAndPop) {
    ThreadSafeStack<int> stack;
    std::thread producer([&stack]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        stack.push(100);
    });
    int value = 0;
    stack.wait_and_pop(value);
    EXPECT_EQ(value, 100);
    producer.join();
}

TEST(ThreadSafeStackTest, PopEmpty) {
    ThreadSafeStack<int> stack;
    auto val = stack.pop();
    EXPECT_EQ(val, nullptr);
}
