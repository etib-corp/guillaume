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

#include "primitives/rectangle.hpp"
#include "test_rectangle.hpp"

TEST(RectangleTest, DefaultConstructor) {
  Rectangle rect;
  EXPECT_EQ(rect.getPoints().size(), 0);
}

TEST(RectangleTest, ParameterizedConstructor) {
  Point topLeft(0.0f, 0.0f, 0.0f);
  Point bottomRight(10.0f, 10.0f, 0.0f);

  Rectangle rect(topLeft, bottomRight);

  EXPECT_EQ(rect.getPoints().size(), 4);
}

TEST(RectangleTest, CorrectPointOrder) {
  Point topLeft(0.0f, 0.0f, 0.0f);
  Point bottomRight(10.0f, 10.0f, 0.0f);

  Rectangle rect(topLeft, bottomRight);
  const auto &points = rect.getPoints();

  // First point: top-left
  EXPECT_FLOAT_EQ(points[0].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[0].y(), 0.0f);
  EXPECT_FLOAT_EQ(points[0].z(), 0.0f);

  // Second point: top-right
  EXPECT_FLOAT_EQ(points[1].x(), 10.0f);
  EXPECT_FLOAT_EQ(points[1].y(), 0.0f);
  EXPECT_FLOAT_EQ(points[1].z(), 0.0f);

  // Third point: bottom-right
  EXPECT_FLOAT_EQ(points[2].x(), 10.0f);
  EXPECT_FLOAT_EQ(points[2].y(), 10.0f);
  EXPECT_FLOAT_EQ(points[2].z(), 0.0f);

  // Fourth point: bottom-left
  EXPECT_FLOAT_EQ(points[3].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[3].y(), 10.0f);
  EXPECT_FLOAT_EQ(points[3].z(), 0.0f);
}

TEST(RectangleTest, InheritsFromPolygon) {
  Rectangle rect;
  Polygon *polygonPtr = &rect;

  EXPECT_NO_THROW(delete new Rectangle());
}

TEST(RectangleTest, NegativeCoordinates) {
  Point topLeft(-5.0f, -5.0f, 0.0f);
  Point bottomRight(5.0f, 5.0f, 0.0f);

  Rectangle rect(topLeft, bottomRight);
  const auto &points = rect.getPoints();

  EXPECT_EQ(points.size(), 4);
  EXPECT_FLOAT_EQ(points[0].x(), -5.0f);
  EXPECT_FLOAT_EQ(points[0].y(), -5.0f);
  EXPECT_FLOAT_EQ(points[2].x(), 5.0f);
  EXPECT_FLOAT_EQ(points[2].y(), 5.0f);
}

TEST(RectangleTest, UnitSquare) {
  Point topLeft(0.0f, 0.0f, 0.0f);
  Point bottomRight(1.0f, 1.0f, 0.0f);

  Rectangle rect(topLeft, bottomRight);
  const auto &points = rect.getPoints();

  EXPECT_EQ(points.size(), 4);

  // Verify all four corners of unit square
  EXPECT_FLOAT_EQ(points[0].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[0].y(), 0.0f);
  EXPECT_FLOAT_EQ(points[1].x(), 1.0f);
  EXPECT_FLOAT_EQ(points[1].y(), 0.0f);
  EXPECT_FLOAT_EQ(points[2].x(), 1.0f);
  EXPECT_FLOAT_EQ(points[2].y(), 1.0f);
  EXPECT_FLOAT_EQ(points[3].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[3].y(), 1.0f);
}

TEST(RectangleTest, LargeRectangle) {
  Point topLeft(0.0f, 0.0f, 0.0f);
  Point bottomRight(1000.0f, 500.0f, 0.0f);

  Rectangle rect(topLeft, bottomRight);
  const auto &points = rect.getPoints();

  EXPECT_EQ(points.size(), 4);
  EXPECT_FLOAT_EQ(points[1].x(), 1000.0f);
  EXPECT_FLOAT_EQ(points[2].y(), 500.0f);
}

TEST(RectangleTest, NonZeroZCoordinate) {
  Point topLeft(0.0f, 0.0f, 5.0f);
  Point bottomRight(10.0f, 10.0f, 5.0f);

  Rectangle rect(topLeft, bottomRight);
  const auto &points = rect.getPoints();

  // Z coordinates should be preserved for 3D positioning
  EXPECT_FLOAT_EQ(points[0].z(), 5.0f);  // topLeft z
  EXPECT_FLOAT_EQ(points[1].z(), 5.0f);  // topRight z (same as topLeft)
  EXPECT_FLOAT_EQ(points[2].z(), 5.0f);  // bottomRight z
  EXPECT_FLOAT_EQ(points[3].z(), 5.0f);  // bottomLeft z (same as bottomRight)
}

TEST(RectangleTest, DecimalCoordinates) {
  Point topLeft(1.5f, 2.5f, 0.0f);
  Point bottomRight(3.7f, 4.8f, 0.0f);

  Rectangle rect(topLeft, bottomRight);
  const auto &points = rect.getPoints();

  EXPECT_EQ(points.size(), 4);
  EXPECT_FLOAT_EQ(points[0].x(), 1.5f);
  EXPECT_FLOAT_EQ(points[0].y(), 2.5f);
  EXPECT_FLOAT_EQ(points[2].x(), 3.7f);
  EXPECT_FLOAT_EQ(points[2].y(), 4.8f);
}
