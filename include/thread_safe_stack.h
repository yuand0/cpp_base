#pragma once
#include <stack>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>
class ThreadSafeStack {
public:
    ThreadSafeStack() = default;
    ~ThreadSafeStack() = default;

    // 禁止拷贝
    ThreadSafeStack(const ThreadSafeStack&) = delete;
    ThreadSafeStack& operator=(const ThreadSafeStack&) = delete;

    void push(T value);
    std::shared_ptr<T> pop();
    void wait_and_pop(T& value);
    bool empty() const;

private:
    std::stack<T> stack_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
};
