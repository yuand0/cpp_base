#include "thread_safe_stack.h"

template<typename T>
void ThreadSafeStack<T>::push(T value) {
    std::lock_guard<std::mutex> lock(mutex_);
    stack_.push(std::move(value));
    cv_.notify_one();
}

template<typename T>
std::shared_ptr<T> ThreadSafeStack<T>::pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stack_.empty()) {
        return nullptr;
    }
    auto result = std::make_shared<T>(std::move(stack_.top()));
    stack_.pop();
    return result;
}

template<typename T>
void ThreadSafeStack<T>::wait_and_pop(T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !stack_.empty(); });
    value = std::move(stack_.top());
    stack_.pop();
}

template<typename T>
bool ThreadSafeStack<T>::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return stack_.empty();
}

// 显式实例化
template class ThreadSafeStack<int>;
