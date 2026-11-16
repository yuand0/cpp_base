#include <gtest/gtest.h>
#include "thread_safe_stack.h"
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

TEST(ThreadSafeStackTest, PushPop) {
    ThreadSafeStack<int> stack;
    stack.push(42);
    auto val = stack.pop();
    ASSERT_TRUE(val.has_value());
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
    EXPECT_FALSE(val.has_value());
}

TEST(ThreadSafeStackTest, StressTest) {
    ThreadSafeStack<int> stack;
    const int num_producers = 10;
    const int num_consumers = 10;
    const int items_per_producer = 100;
    const int total_items = num_producers * items_per_producer;
    
    std::atomic<int> consumed_count{0};
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    for (int i = 0; i < num_producers; ++i) {
        producers.emplace_back([&stack, items_per_producer]() {
            for (int j = 0; j < items_per_producer; ++j) {
                stack.push(j);
            }
        });
    }
    
    for (int i = 0; i < num_consumers; ++i) {
        consumers.emplace_back([&stack, &consumed_count]() {
            while (consumed_count.load() < 1000) {
                auto val = stack.pop();
                if (val.has_value()) {
                    consumed_count.fetch_add(1);
                } else {
                    std::this_thread::yield();
                }
            }
        });
    }
    
    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();
    
    EXPECT_EQ(consumed_count.load(), total_items);
    EXPECT_TRUE(stack.empty());
}
