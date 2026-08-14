#include "intarray.h"
#include <algorithm>

IntArray::IntArray(size_t size) : data_(new int[size]), size_(size) {
    std::fill(data_, data_ + size, 0);
}

IntArray::~IntArray() { delete[] data_; }

IntArray::IntArray(const IntArray& other)
    : data_(new int[other.size_]), size_(other.size_) {
    std::copy(other.data_, other.data_ + size_, data_);
}

IntArray::IntArray(IntArray&& other) noexcept
    : data_(other.data_), size_(other.size_) {
    other.data_ = nullptr;
    other.size_ = 0;
}

IntArray& IntArray::operator=(const IntArray& other) {
    if (this != &other) {
        delete[] data_;
        data_ = new int[other.size_];
        size_ = other.size_;
        std::copy(other.data_, other.data_ + size_, data_);
    }
    return *this;
}

IntArray& IntArray::operator=(IntArray&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        other.data_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}
