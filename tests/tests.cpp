#include "../lib/buffer.h"
#include <C:\Users\User\labwork-8-4BENE\cmake-build-debug\_deps\googletest-src\googletest\include\gtest\gtest.h>

TEST(CCircularBufferTest, ConstructorTest) {
CCircularBuffer<int> buffer(10);
EXPECT_EQ(buffer.size(), 0);
EXPECT_EQ(buffer.capacity(), 10);
EXPECT_TRUE(buffer.empty());

CCircularBuffer<int> buffer2(10, 5);
EXPECT_EQ(buffer2.size(), 10);
for (size_t i = 0; i < 10; i++) {
EXPECT_EQ(buffer2[i], 5);
}

CCircularBuffer<int> buffer3(buffer2);
EXPECT_TRUE(buffer3 == buffer2);
EXPECT_FALSE(buffer3 == buffer);
}

TEST(CCircularBufferTest, CopyAssignmentOperator) {
CCircularBuffer<int> buf(10);
buf.push_back(1);
buf.push_back(2);
buf.push_back(3);
CCircularBuffer<int> copy;
copy = buf;
EXPECT_EQ(copy.capacity(), 10);
EXPECT_EQ(copy.size(), 3);
EXPECT_EQ(copy[0], 1);
EXPECT_EQ(copy[1], 2);
EXPECT_EQ(copy[2], 3);
}

TEST(CCircularBufferTest, PushBack) {
CCircularBuffer<int> buf(3);
buf.push_back(1);
buf.push_back(2);
buf.push_back(3);
EXPECT_EQ(buf.size(), 3);
EXPECT_EQ(buf[0], 1);
EXPECT_EQ(buf[1], 2);
EXPECT_EQ(buf[2], 3);
}

TEST(CCircularBufferTest, PushFront) {
CCircularBuffer<int> buf(2);
buf.push_front(2);
buf.push_front(3);
EXPECT_EQ(buf.size(), 2);
EXPECT_EQ(buf[1], 3);
}


TEST(CCircularBufferTest, PopFront) {
CCircularBuffer<int> buf(3);
buf.push_back(1);
buf.push_back(2);
buf.push_back(3);
buf.pop_front();
EXPECT_EQ(buf.capacity(), 3);
EXPECT_EQ(buf.size(), 2);
EXPECT_EQ(buf[0], 2);
}

TEST(CCircularBufferTest, PopBack) {
CCircularBuffer<int> buf(2);
buf.push_back(1);
buf.push_back(2);
buf.pop_back();
EXPECT_EQ(buf.capacity(), 2);
EXPECT_EQ(buf.size(), 1);
EXPECT_EQ(buf[0], 1);
}

TEST(CCircularBufferTest, Resize) {
CCircularBuffer<int> buf(2);
buf.push_back(1);
buf.push_back(2);
buf.resize(4);
EXPECT_EQ(buf.capacity(), 4);
EXPECT_EQ(buf.size(), 4);
EXPECT_EQ(buf[0], 1);
EXPECT_EQ(buf[1], 2);
EXPECT_EQ(buf[2], 0);
EXPECT_EQ(buf[3], 0);
}

TEST(CCircularBufferTest, Reserve) {
CCircularBuffer<int> buf(2);
buf.push_back(1);
buf.push_back(2);
buf.reserve(4);
EXPECT_EQ(buf.capacity(), 4);
EXPECT_EQ(buf.size(), 2);
EXPECT_EQ(buf[0], 1);
EXPECT_EQ(buf[1], 2);
}

TEST(CCircularBufferTest, Iterator) {
CCircularBuffer<int> buffer(3);
buffer.push_back(1);
buffer.push_back(2);
buffer.push_back(3);
int i = 1;
for (auto it = buffer.begin(); it != buffer.end(); ++it) {
EXPECT_EQ(*it, i);
++i;
}
}

TEST(CCircularBufferTest, Clear) {
const size_t buffer_size = 5;
CCircularBuffer<int> buffer(buffer_size);
buffer.push_back(1);
buffer.push_back(2);
buffer.push_back(3);

buffer.clear();

EXPECT_EQ(buffer.size(), 0);
EXPECT_EQ(buffer.capacity(), buffer_size);
EXPECT_TRUE(buffer.empty());
}

TEST(CCircularBufferExtTest, PushBack) {
CCircularBufferExt<int> buf(3);
buf.push_back(1);
buf.push_back(2);
EXPECT_EQ(buf.capacity(), 3);
buf.push_back(3);
EXPECT_EQ(buf.size(), 3);
EXPECT_EQ(buf[0], 1);
EXPECT_EQ(buf[1], 2);
EXPECT_EQ(buf[2], 3);
EXPECT_EQ(buf.capacity(), 6);
}

TEST(CCircularBufferExtTest, PushFront) {
CCircularBufferExt<int> buf(3);
buf.push_front(2);
buf.push_front(3);
EXPECT_EQ(buf.capacity(), 3);
buf.push_front(4);
EXPECT_EQ(buf.size(), 3);
EXPECT_EQ(buf.capacity(), 6);
}


TEST(CCircularBufferExtTest, PopFront) {
CCircularBufferExt<int> buf(2);
buf.push_back(1);
buf.push_back(2);
EXPECT_EQ(buf.capacity(), 4);
buf.pop_front();
EXPECT_EQ(buf.capacity(), 2);
EXPECT_EQ(buf.size(), 1);
EXPECT_EQ(buf[0], 2);
}

TEST(CCircularBufferExtTest, PopBack) {
CCircularBufferExt<int> buf(2);
buf.push_back(1);
buf.push_back(2);
EXPECT_EQ(buf.capacity(), 4);
buf.pop_back();
EXPECT_EQ(buf.capacity(), 2);
EXPECT_EQ(buf.size(), 1);
EXPECT_EQ(buf[0], 1);
}
