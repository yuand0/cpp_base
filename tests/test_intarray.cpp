#include <gtest/gtest.h>
#include "intarray.h"

TEST(IntArrayTest, Construct) {
    IntArray a(10);
    EXPECT_EQ(a.size(), 10);
}

TEST(IntArrayTest, Copy) {
    IntArray a(5);
    IntArray b(a);
    EXPECT_EQ(b.size(), 5);
}

TEST(IntArrayTest, CopyAssign) {
    IntArray a(3);
    IntArray b(6);
    b = a;
    EXPECT_EQ(b.size(), 3);
}

TEST(IntArrayTest, Move) {
    IntArray a(7);
    IntArray b(std::move(a));
    EXPECT_EQ(b.size(), 7);
    EXPECT_EQ(a.size(), 0);
}

TEST(IntArrayTest, MoveAssign) {
    IntArray a(4);
    IntArray b(8);
    b = std::move(a);
    EXPECT_EQ(b.size(), 4);
    EXPECT_EQ(a.size(), 0);
}

TEST(IntArrayTest, CopySelfAssign) {
    IntArray a(10);
    a.data()[0] = 42;
    a = a;
    EXPECT_EQ(a.size(), 10);
    EXPECT_EQ(a.data()[0], 42);
}

TEST(IntArrayTest, MoveSelfAssign) {
    IntArray a(10);
    a.data()[0] = 42;
    a = std::move(a);
    EXPECT_EQ(a.size(), 10);
}
