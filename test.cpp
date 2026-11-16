#include <iostream>
#include <future>
#include <thread>

int try1() {
    std::packaged_task<int()> task([]{ return 42; });
    std::future<int> fut = task.get_future();
    {
        std::packaged_task<int()> tmp(std::move(task));
        // tmp 析构时，若任务未执行，会向 future 写入 broken_promise
    }
    std::cout << fut.get() << std::endl;
    return 0;
}

int main() {
    return try1();
}