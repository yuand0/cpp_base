#pragma once
#include <cstddef>

class IntArray {
public:
    explicit IntArray(size_t size);
    ~IntArray();
    IntArray(const IntArray& other);
    IntArray(IntArray&& other) noexcept;
    IntArray& operator=(const IntArray& other);
    IntArray& operator=(IntArray&& other) noexcept;
    int* data() const { return data_; }
    size_t size() const { return size_; }
private:
    int* data_;
    size_t size_;
};
