/*
 Copyright (c) 2025 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <gtest/gtest.h>

#include "test_vector.hpp"
#include "vector.hpp"

TEST(VectorTest, DefaultConstructor) {
  Vector<float, 3> vec;
  EXPECT_FLOAT_EQ(vec[0], 0.0f);
  EXPECT_FLOAT_EQ(vec[1], 0.0f);
  EXPECT_FLOAT_EQ(vec[2], 0.0f);
}

TEST(VectorTest, InitializerListConstructor) {
  Vector<float, 3> vec({1.0f, 2.0f, 3.0f});
  EXPECT_FLOAT_EQ(vec[0], 1.0f);
  EXPECT_FLOAT_EQ(vec[1], 2.0f);
  EXPECT_FLOAT_EQ(vec[2], 3.0f);
}

TEST(VectorTest, PartialInitializerList) {
  Vector<float, 5> vec({1.0f, 2.0f});
  EXPECT_FLOAT_EQ(vec[0], 1.0f);
  EXPECT_FLOAT_EQ(vec[1], 2.0f);
  // Remaining elements should be default-initialized
  // Note: Vector uses std::array which default-initializes to 0 for built-in types
}

TEST(VectorTest, ElementAccess) {
  Vector<float, 3> vec({4.0f, 5.0f, 6.0f});
  EXPECT_FLOAT_EQ(vec[0], 4.0f);
  EXPECT_FLOAT_EQ(vec[1], 5.0f);
  EXPECT_FLOAT_EQ(vec[2], 6.0f);
}

TEST(VectorTest, ElementModification) {
  Vector<float, 3> vec;
  vec[0] = 10.0f;
  vec[1] = 20.0f;
  vec[2] = 30.0f;

  EXPECT_FLOAT_EQ(vec[0], 10.0f);
  EXPECT_FLOAT_EQ(vec[1], 20.0f);
  EXPECT_FLOAT_EQ(vec[2], 30.0f);
}

TEST(VectorTest, Addition) {
  Vector<float, 3> vec1({1.0f, 2.0f, 3.0f});
  Vector<float, 3> vec2({4.0f, 5.0f, 6.0f});

  auto result = vec1 + vec2;
  EXPECT_FLOAT_EQ(result[0], 5.0f);
  EXPECT_FLOAT_EQ(result[1], 7.0f);
  EXPECT_FLOAT_EQ(result[2], 9.0f);
}

TEST(VectorTest, Subtraction) {
  Vector<float, 3> vec1({10.0f, 8.0f, 6.0f});
  Vector<float, 3> vec2({1.0f, 2.0f, 3.0f});

  auto result = vec1 - vec2;
  EXPECT_FLOAT_EQ(result[0], 9.0f);
  EXPECT_FLOAT_EQ(result[1], 6.0f);
  EXPECT_FLOAT_EQ(result[2], 3.0f);
}

TEST(VectorTest, ScalarMultiplication) {
  Vector<float, 3> vec({2.0f, 3.0f, 4.0f});

  auto result = vec * 3.0f;
  EXPECT_FLOAT_EQ(result[0], 6.0f);
  EXPECT_FLOAT_EQ(result[1], 9.0f);
  EXPECT_FLOAT_EQ(result[2], 12.0f);
}

TEST(VectorTest, DotProduct) {
  Vector<float, 3> vec1({1.0f, 2.0f, 3.0f});
  Vector<float, 3> vec2({4.0f, 5.0f, 6.0f});

  float result = vec1.dot(vec2);
  // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
  EXPECT_FLOAT_EQ(result, 32.0f);
}

TEST(VectorTest, DotProductZero) {
  Vector<float, 3> vec1({1.0f, 0.0f, 0.0f});
  Vector<float, 3> vec2({0.0f, 1.0f, 0.0f});

  float result = vec1.dot(vec2);
  EXPECT_FLOAT_EQ(result, 0.0f);
}

TEST(VectorTest, Magnitude) {
  Vector<float, 3> vec({3.0f, 4.0f, 0.0f});

  float mag = vec.magnitude();
  EXPECT_FLOAT_EQ(mag, 5.0f); // sqrt(9 + 16 + 0) = 5
}

TEST(VectorTest, MagnitudeZero) {
  Vector<float, 3> vec({0.0f, 0.0f, 0.0f});

  float mag = vec.magnitude();
  EXPECT_FLOAT_EQ(mag, 0.0f);
}

TEST(VectorTest, Normalize) {
  Vector<float, 3> vec({3.0f, 4.0f, 0.0f});

  auto normalized = vec.normalize();
  EXPECT_FLOAT_EQ(normalized[0], 0.6f);
  EXPECT_FLOAT_EQ(normalized[1], 0.8f);
  EXPECT_FLOAT_EQ(normalized[2], 0.0f);

  // Check magnitude is 1
  EXPECT_NEAR(normalized.magnitude(), 1.0f, 0.0001f);
}

TEST(VectorTest, NormalizeZeroVector) {
  Vector<float, 3> vec({0.0f, 0.0f, 0.0f});

  auto normalized = vec.normalize();
  EXPECT_FLOAT_EQ(normalized[0], 0.0f);
  EXPECT_FLOAT_EQ(normalized[1], 0.0f);
  EXPECT_FLOAT_EQ(normalized[2], 0.0f);
}

TEST(VectorTest, Size) {
  Vector<float, 3> vec3;
  Vector<int, 5> vec5;

  EXPECT_EQ(vec3.size(), 3);
  EXPECT_EQ(vec5.size(), 5);
}

TEST(VectorTest, IntegerVector) {
  Vector<int, 3> vec({1, 2, 3});
  Vector<int, 3> vec2({4, 5, 6});

  auto result = vec + vec2;
  EXPECT_EQ(result[0], 5);
  EXPECT_EQ(result[1], 7);
  EXPECT_EQ(result[2], 9);
}

TEST(VectorTest, DoubleVector) {
  Vector<double, 2> vec({1.5, 2.5});
  Vector<double, 2> vec2({3.5, 4.5});

  auto result = vec + vec2;
  EXPECT_DOUBLE_EQ(result[0], 5.0);
  EXPECT_DOUBLE_EQ(result[1], 7.0);
}

TEST(VectorTest, NegativeValues) {
  Vector<float, 3> vec({-1.0f, -2.0f, -3.0f});

  EXPECT_FLOAT_EQ(vec[0], -1.0f);
  EXPECT_FLOAT_EQ(vec[1], -2.0f);
  EXPECT_FLOAT_EQ(vec[2], -3.0f);
}

TEST(VectorTest, ScalarMultiplicationNegative) {
  Vector<float, 3> vec({1.0f, 2.0f, 3.0f});

  auto result = vec * -2.0f;
  EXPECT_FLOAT_EQ(result[0], -2.0f);
  EXPECT_FLOAT_EQ(result[1], -4.0f);
  EXPECT_FLOAT_EQ(result[2], -6.0f);
}

TEST(VectorTest, HighDimensionalVector) {
  Vector<float, 10> vec;
  for (size_t i = 0; i < 10; ++i) {
    vec[i] = static_cast<float>(i);
  }

  for (size_t i = 0; i < 10; ++i) {
    EXPECT_FLOAT_EQ(vec[i], static_cast<float>(i));
  }
}
