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

#include "point.hpp"
#include "test_point.hpp"

TEST(PointTest, DefaultConstructor) {
  Point point;
  EXPECT_FLOAT_EQ(point.x(), 0.0f);
  EXPECT_FLOAT_EQ(point.y(), 0.0f);
  EXPECT_FLOAT_EQ(point.z(), 0.0f);
}

TEST(PointTest, ParameterizedConstructor) {
  Point point(1.0f, 2.0f, 3.0f);
  EXPECT_FLOAT_EQ(point.x(), 1.0f);
  EXPECT_FLOAT_EQ(point.y(), 2.0f);
  EXPECT_FLOAT_EQ(point.z(), 3.0f);
}

TEST(PointTest, NegativeCoordinates) {
  Point point(-1.5f, -2.5f, -3.5f);
  EXPECT_FLOAT_EQ(point.x(), -1.5f);
  EXPECT_FLOAT_EQ(point.y(), -2.5f);
  EXPECT_FLOAT_EQ(point.z(), -3.5f);
}

TEST(PointTest, VectorInheritance) {
  Point point(4.0f, 5.0f, 6.0f);
  // Test that Point inherits from Vector<float, 3>
  EXPECT_FLOAT_EQ(point[0], 4.0f);
  EXPECT_FLOAT_EQ(point[1], 5.0f);
  EXPECT_FLOAT_EQ(point[2], 6.0f);
}

TEST(PointTest, VectorOperations) {
  Point point1(1.0f, 2.0f, 3.0f);
  Point point2(4.0f, 5.0f, 6.0f);

  // Test addition
  auto result = point1 + point2;
  EXPECT_FLOAT_EQ(result[0], 5.0f);
  EXPECT_FLOAT_EQ(result[1], 7.0f);
  EXPECT_FLOAT_EQ(result[2], 9.0f);
}

TEST(PointTest, VectorSubtraction) {
  Point point1(4.0f, 5.0f, 6.0f);
  Point point2(1.0f, 2.0f, 3.0f);

  auto result = point1 - point2;
  EXPECT_FLOAT_EQ(result[0], 3.0f);
  EXPECT_FLOAT_EQ(result[1], 3.0f);
  EXPECT_FLOAT_EQ(result[2], 3.0f);
}

TEST(PointTest, ScalarMultiplication) {
  Point point(2.0f, 3.0f, 4.0f);

  auto result = point * 2.0f;
  EXPECT_FLOAT_EQ(result[0], 4.0f);
  EXPECT_FLOAT_EQ(result[1], 6.0f);
  EXPECT_FLOAT_EQ(result[2], 8.0f);
}

TEST(PointTest, MixedCoordinates) {
  Point point(0.0f, 10.5f, -5.2f);
  EXPECT_FLOAT_EQ(point.x(), 0.0f);
  EXPECT_FLOAT_EQ(point.y(), 10.5f);
  EXPECT_FLOAT_EQ(point.z(), -5.2f);
}

TEST(PointTest, ElementAccess) {
  Point point(7.0f, 8.0f, 9.0f);

  // Verify x(), y(), z() return same as operator[]
  EXPECT_FLOAT_EQ(point.x(), point[0]);
  EXPECT_FLOAT_EQ(point.y(), point[1]);
  EXPECT_FLOAT_EQ(point.z(), point[2]);
}
