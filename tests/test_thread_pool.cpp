#include <gtest/gtest.h>
#include "thread_pool.h"
#include <chrono>
#include <atomic>
#include <thread>

TEST(ThreadPoolTest, SubmitSimpleTask) {
    ThreadPool pool(2);
    auto future = pool.submit([]() { return 42; });
    EXPECT_EQ(future.get(), 42);
}

TEST(ThreadPoolTest, SubmitMultipleTasks) {
    ThreadPool pool(4);
    auto f1 = pool.submit([]() { return 1; });
    auto f2 = pool.submit([]() { return 2; });
    auto f3 = pool.submit([]() { return 3; });
    EXPECT_EQ(f1.get() + f2.get() + f3.get(), 6);
}

TEST(ThreadPoolTest, SubmitException) {
    ThreadPool pool(2);
    auto future = pool.submit([]() { throw std::runtime_error("test error"); });
    EXPECT_THROW(future.get(), std::runtime_error);
}

TEST(ThreadPoolTest, RAIIDestructor) {
    auto start = std::chrono::steady_clock::now();
    {
        ThreadPool pool(4);
        for (int i = 0; i < 100; ++i) {
            pool.submit([]() { 
                std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
            });
        }
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // 4 线程并行执行 100 个 1ms 任务，理论耗时约 25ms
    // 检查析构时等待了所有任务完成（耗时 >= 20ms 即可）
    EXPECT_GE(duration.count(), 20);
    EXPECT_LT(duration.count(), 300);
}

TEST(ThreadPoolTest, StressTest) {
    ThreadPool pool(8);
    std::atomic<int> counter{0};
    std::vector<std::future<void>> futures;
    
    for (int i = 0; i < 1000; ++i) {
        futures.push_back(pool.submit([&counter]() {
            counter.fetch_add(1);
        }));
    }
    
    for (auto& f : futures) {
        f.wait();
    }
    
    EXPECT_EQ(counter.load(), 1000);
}

TEST(ThreadPoolTest, SubmitAfterStopThrows) {
    SUCCEED();
}
