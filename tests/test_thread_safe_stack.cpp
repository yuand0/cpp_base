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

// 压力测试：10 个生产者，10 个消费者，共 1000 个元素
TEST(ThreadSafeStackTest, StressTest) {
    ThreadSafeStack<int> stack;
    const int num_producers = 10;
    const int num_consumers = 10;
    const int items_per_producer = 100;
    const int total_items = num_producers * items_per_producer;
    
    std::atomic<int> consumed_count{0};
    std::atomic<bool> producers_done{false};
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    // 生产者
    for (int i = 0; i < num_producers; ++i) {
        producers.emplace_back([&stack, items_per_producer]() {
            for (int j = 0; j < items_per_producer; ++j) {
                stack.push(j);
            }
        });
    }
    
    // 消费者：忙等 + pop
    for (int i = 0; i < num_consumers; ++i) {
        consumers.emplace_back([&stack, &consumed_count, &producers_done, total_items]() {
            while (consumed_count.load() < total_items) {
                auto val = stack.pop();
                if (val != nullptr) {
                    consumed_count.fetch_add(1);
                } else {
                    // 栈空且生产者未完成，让出CPU
                    if (!producers_done.load()) {
                        std::this_thread::yield();
                    } else {
                        // 生产者已完成但栈空，可能元素还没被推完，短暂等待
                        std::this_thread::sleep_for(std::chrono::microseconds(10));
                    }
                }
            }
        });
    }
    
    // 等待所有生产者完成
    for (auto& t : producers) t.join();
    producers_done = true;
    
    // 等待所有消费者完成（给足够时间）
    for (auto& t : consumers) t.join();
    
    EXPECT_EQ(consumed_count.load(), total_items);
    EXPECT_TRUE(stack.empty());
}
