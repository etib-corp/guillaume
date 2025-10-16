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

#include "primitives/triangle.hpp"
#include "test_triangle.hpp"

TEST(TriangleTest, DefaultConstructor) {
  Triangle triangle;
  EXPECT_EQ(triangle.getPoints().size(), 0);
}

TEST(TriangleTest, ParameterizedConstructor) {
  Point p1(0.0f, 0.0f, 0.0f);
  Point p2(1.0f, 0.0f, 0.0f);
  Point p3(0.5f, 1.0f, 0.0f);

  Triangle triangle(p1, p2, p3);

  EXPECT_EQ(triangle.getPoints().size(), 3);
}

TEST(TriangleTest, CorrectPointOrder) {
  Point p1(0.0f, 0.0f, 0.0f);
  Point p2(1.0f, 0.0f, 0.0f);
  Point p3(0.5f, 1.0f, 0.0f);

  Triangle triangle(p1, p2, p3);
  const auto &points = triangle.getPoints();

  // First point
  EXPECT_FLOAT_EQ(points[0].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[0].y(), 0.0f);
  EXPECT_FLOAT_EQ(points[0].z(), 0.0f);

  // Second point
  EXPECT_FLOAT_EQ(points[1].x(), 1.0f);
  EXPECT_FLOAT_EQ(points[1].y(), 0.0f);
  EXPECT_FLOAT_EQ(points[1].z(), 0.0f);

  // Third point
  EXPECT_FLOAT_EQ(points[2].x(), 0.5f);
  EXPECT_FLOAT_EQ(points[2].y(), 1.0f);
  EXPECT_FLOAT_EQ(points[2].z(), 0.0f);
}

TEST(TriangleTest, InheritsFromPolygon) {
  Triangle triangle;
  (void)triangle; // avoid unused-variable warning; test checks construction/destruction

  EXPECT_NO_THROW(delete new Triangle());
}

TEST(TriangleTest, EquilateralTriangle) {
  // Create an approximate equilateral triangle
  Point p1(0.0f, 0.0f, 0.0f);
  Point p2(1.0f, 0.0f, 0.0f);
  Point p3(0.5f, 0.866f, 0.0f);

  Triangle triangle(p1, p2, p3);
  const auto &points = triangle.getPoints();

  EXPECT_EQ(points.size(), 3);
  EXPECT_FLOAT_EQ(points[0].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[1].x(), 1.0f);
  EXPECT_NEAR(points[2].y(), 0.866f, 0.001f);
}

TEST(TriangleTest, RightTriangle) {
  Point p1(0.0f, 0.0f, 0.0f);
  Point p2(3.0f, 0.0f, 0.0f);
  Point p3(0.0f, 4.0f, 0.0f);

  Triangle triangle(p1, p2, p3);
  const auto &points = triangle.getPoints();

  EXPECT_EQ(points.size(), 3);
  EXPECT_FLOAT_EQ(points[0].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[0].y(), 0.0f);
  EXPECT_FLOAT_EQ(points[1].x(), 3.0f);
  EXPECT_FLOAT_EQ(points[2].y(), 4.0f);
}

TEST(TriangleTest, NegativeCoordinates) {
  Point p1(-1.0f, -1.0f, 0.0f);
  Point p2(1.0f, -1.0f, 0.0f);
  Point p3(0.0f, 1.0f, 0.0f);

  Triangle triangle(p1, p2, p3);
  const auto &points = triangle.getPoints();

  EXPECT_EQ(points.size(), 3);
  EXPECT_FLOAT_EQ(points[0].x(), -1.0f);
  EXPECT_FLOAT_EQ(points[0].y(), -1.0f);
  EXPECT_FLOAT_EQ(points[1].x(), 1.0f);
  EXPECT_FLOAT_EQ(points[2].y(), 1.0f);
}

TEST(TriangleTest, NonZeroZCoordinates) {
  Point p1(0.0f, 0.0f, 5.0f);
  Point p2(1.0f, 0.0f, 5.0f);
  Point p3(0.5f, 1.0f, 5.0f);

  Triangle triangle(p1, p2, p3);
  const auto &points = triangle.getPoints();

  EXPECT_EQ(points.size(), 3);
  EXPECT_FLOAT_EQ(points[0].z(), 5.0f);
  EXPECT_FLOAT_EQ(points[1].z(), 5.0f);
  EXPECT_FLOAT_EQ(points[2].z(), 5.0f);
}

TEST(TriangleTest, DecimalCoordinates) {
  Point p1(1.5f, 2.5f, 3.5f);
  Point p2(4.7f, 5.8f, 6.9f);
  Point p3(7.1f, 8.2f, 9.3f);

  Triangle triangle(p1, p2, p3);
  const auto &points = triangle.getPoints();

  EXPECT_EQ(points.size(), 3);
  EXPECT_FLOAT_EQ(points[0].x(), 1.5f);
  EXPECT_FLOAT_EQ(points[0].y(), 2.5f);
  EXPECT_FLOAT_EQ(points[0].z(), 3.5f);
  EXPECT_FLOAT_EQ(points[1].x(), 4.7f);
  EXPECT_FLOAT_EQ(points[2].x(), 7.1f);
}

TEST(TriangleTest, IsoscelesTriangle) {
  Point p1(0.0f, 0.0f, 0.0f);
  Point p2(2.0f, 0.0f, 0.0f);
  Point p3(1.0f, 2.0f, 0.0f);

  Triangle triangle(p1, p2, p3);
  const auto &points = triangle.getPoints();

  EXPECT_EQ(points.size(), 3);
  EXPECT_FLOAT_EQ(points[0].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[1].x(), 2.0f);
  EXPECT_FLOAT_EQ(points[2].x(), 1.0f);
  EXPECT_FLOAT_EQ(points[2].y(), 2.0f);
}

TEST(TriangleTest, DegenerateTriangle) {
  // All points on a line (degenerate triangle)
  Point p1(0.0f, 0.0f, 0.0f);
  Point p2(1.0f, 1.0f, 0.0f);
  Point p3(2.0f, 2.0f, 0.0f);

  Triangle triangle(p1, p2, p3);
  const auto &points = triangle.getPoints();

  // Should still create triangle with 3 points
  EXPECT_EQ(points.size(), 3);
  EXPECT_FLOAT_EQ(points[0].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[1].x(), 1.0f);
  EXPECT_FLOAT_EQ(points[2].x(), 2.0f);
}
