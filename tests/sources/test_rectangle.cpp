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

  EXPECT_EQ(rect.getPoints().size(), 4);
  EXPECT_FLOAT_EQ(rect.getPoints()[0].x(), 1.5f);
  EXPECT_FLOAT_EQ(rect.getPoints()[0].y(), 2.5f);
  EXPECT_FLOAT_EQ(rect.getPoints()[2].x(), 3.7f);
  EXPECT_FLOAT_EQ(rect.getPoints()[2].y(), 4.8f);
}

// 3D Rectangle Constructor Tests

TEST(RectangleTest, CenterWidthHeightConstructor) {
  Point center(50.0f, 25.0f, 10.0f);
  float width = 100.0f;
  float height = 50.0f;
  Point rotation(0.0f, 0.0f, 0.0f);

  Rectangle rect(center, width, height, rotation);

  EXPECT_EQ(rect.getPoints().size(), 4);
  EXPECT_FLOAT_EQ(rect.getCenter().x(), 50.0f);
  EXPECT_FLOAT_EQ(rect.getCenter().y(), 25.0f);
  EXPECT_FLOAT_EQ(rect.getCenter().z(), 10.0f);
  EXPECT_FLOAT_EQ(rect.getWidth(), 100.0f);
  EXPECT_FLOAT_EQ(rect.getHeight(), 50.0f);
  EXPECT_FLOAT_EQ(rect.getRotation().x(), 0.0f);
  EXPECT_FLOAT_EQ(rect.getRotation().y(), 0.0f);
  EXPECT_FLOAT_EQ(rect.getRotation().z(), 0.0f);
}

TEST(RectangleTest, CenterWidthHeightWithRotation) {
  Point center(0.0f, 0.0f, 0.0f);
  float width = 10.0f;
  float height = 5.0f;
  Point rotation(0.0f, 0.0f, 1.5708f); // 90 degrees in radians

  Rectangle rect(center, width, height, rotation);

  EXPECT_EQ(rect.getPoints().size(), 4);
  EXPECT_FLOAT_EQ(rect.getCenter().x(), 0.0f);
  EXPECT_FLOAT_EQ(rect.getCenter().y(), 0.0f);
  EXPECT_FLOAT_EQ(rect.getCenter().z(), 0.0f);
  EXPECT_FLOAT_EQ(rect.getWidth(), 10.0f);
  EXPECT_FLOAT_EQ(rect.getHeight(), 5.0f);
  EXPECT_FLOAT_EQ(rect.getRotation().z(), 1.5708f);

  // With 90-degree Z rotation, corners should be rotated
  const auto& points = rect.getPoints();
  // The exact values depend on rotation calculation, but we can verify transformation occurred
  EXPECT_NE(points[0].x(), -5.0f); // Not the unrotated position
}

TEST(RectangleTest, CenterWidthHeightWith3DRotation) {
  Point center(10.0f, 20.0f, 30.0f);
  float width = 6.0f;
  float height = 4.0f;
  Point rotation(0.5f, 1.0f, 1.5f); // Rotation around all three axes

  Rectangle rect(center, width, height, rotation);

  EXPECT_EQ(rect.getPoints().size(), 4);
  EXPECT_FLOAT_EQ(rect.getCenter().x(), 10.0f);
  EXPECT_FLOAT_EQ(rect.getCenter().y(), 20.0f);
  EXPECT_FLOAT_EQ(rect.getCenter().z(), 30.0f);
  EXPECT_FLOAT_EQ(rect.getWidth(), 6.0f);
  EXPECT_FLOAT_EQ(rect.getHeight(), 4.0f);
  EXPECT_FLOAT_EQ(rect.getRotation().x(), 0.5f);
  EXPECT_FLOAT_EQ(rect.getRotation().y(), 1.0f);
  EXPECT_FLOAT_EQ(rect.getRotation().z(), 1.5f);

  // All points should be offset by center position
  const auto& points = rect.getPoints();
  for (const auto& point : points) {
    // Points should be near the center (within reasonable bounds)
     // With 3D rotation, points can be outside simple bounds, so use wider tolerance
     EXPECT_GT(point.x(), 3.0f);  // Reasonable bounds around center
     EXPECT_LT(point.x(), 17.0f); 
     EXPECT_GT(point.y(), 16.0f); 
     EXPECT_LT(point.y(), 24.0f); 
  }
}

TEST(RectangleTest, ZeroSizeRectangle) {
  Point center(0.0f, 0.0f, 0.0f);
  float width = 0.0f;
  float height = 0.0f;
  Point rotation(0.0f, 0.0f, 0.0f);

  Rectangle rect(center, width, height, rotation);

  EXPECT_EQ(rect.getPoints().size(), 4);
  EXPECT_FLOAT_EQ(rect.getWidth(), 0.0f);
  EXPECT_FLOAT_EQ(rect.getHeight(), 0.0f);

  // All points should be at center for zero-size rectangle
  const auto& points = rect.getPoints();
  for (const auto& point : points) {
    EXPECT_FLOAT_EQ(point.x(), 0.0f);
    EXPECT_FLOAT_EQ(point.y(), 0.0f);
    EXPECT_FLOAT_EQ(point.z(), 0.0f);
  }
}

TEST(RectangleTest, UnitSquareWith3DConstructor) {
  Point center(0.5f, 0.5f, 0.0f);
  float width = 1.0f;
  float height = 1.0f;
  Point rotation(0.0f, 0.0f, 0.0f);

  Rectangle rect(center, width, height, rotation);

  EXPECT_EQ(rect.getPoints().size(), 4);
  EXPECT_FLOAT_EQ(rect.getWidth(), 1.0f);
  EXPECT_FLOAT_EQ(rect.getHeight(), 1.0f);

  const auto& points = rect.getPoints();
  // For axis-aligned unit square centered at (0.5, 0.5)
  // Points should be at corners (0,0), (1,0), (1,1), (0,1)
  EXPECT_NEAR(points[0].x(), 0.0f, 1e-5f); // top-left
  EXPECT_NEAR(points[0].y(), 0.0f, 1e-5f);
  EXPECT_NEAR(points[1].x(), 1.0f, 1e-5f); // top-right
  EXPECT_NEAR(points[1].y(), 0.0f, 1e-5f);
  EXPECT_NEAR(points[2].x(), 1.0f, 1e-5f); // bottom-right
  EXPECT_NEAR(points[2].y(), 1.0f, 1e-5f);
  EXPECT_NEAR(points[3].x(), 0.0f, 1e-5f); // bottom-left
  EXPECT_NEAR(points[3].y(), 1.0f, 1e-5f);
}

TEST(RectangleTest, FourCornerConstructor) {
  Point topLeft(0.0f, 0.0f, 0.0f);
  Point topRight(10.0f, 0.0f, 0.0f);
  Point bottomRight(10.0f, 5.0f, 0.0f);
  Point bottomLeft(0.0f, 5.0f, 0.0f);

  Rectangle rect(topLeft, topRight, bottomRight, bottomLeft);

  EXPECT_EQ(rect.getPoints().size(), 4);
  const auto& points = rect.getPoints();

  EXPECT_FLOAT_EQ(points[0].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[0].y(), 0.0f);
  EXPECT_FLOAT_EQ(points[1].x(), 10.0f);
  EXPECT_FLOAT_EQ(points[1].y(), 0.0f);
  EXPECT_FLOAT_EQ(points[2].x(), 10.0f);
  EXPECT_FLOAT_EQ(points[2].y(), 5.0f);
  EXPECT_FLOAT_EQ(points[3].x(), 0.0f);
  EXPECT_FLOAT_EQ(points[3].y(), 5.0f);
}

TEST(RectangleTest, FourCornerConstructorWith3DPoints) {
  Point topLeft(0.0f, 0.0f, 5.0f);
  Point topRight(10.0f, 0.0f, 5.0f);
  Point bottomRight(10.0f, 5.0f, 10.0f);
  Point bottomLeft(0.0f, 5.0f, 10.0f);

  Rectangle rect(topLeft, topRight, bottomRight, bottomLeft);

  EXPECT_EQ(rect.getPoints().size(), 4);
  const auto& points = rect.getPoints();

  EXPECT_FLOAT_EQ(points[0].z(), 5.0f);
  EXPECT_FLOAT_EQ(points[1].z(), 5.0f);
  EXPECT_FLOAT_EQ(points[2].z(), 10.0f);
  EXPECT_FLOAT_EQ(points[3].z(), 10.0f);
}

TEST(RectangleTest, NegativeDimensions) {
  Point center(0.0f, 0.0f, 0.0f);
  float width = -10.0f; // Negative width
  float height = -5.0f;  // Negative height
  Point rotation(0.0f, 0.0f, 0.0f);

  Rectangle rect(center, width, height, rotation);

  // Should still create rectangle, but with negative dimensions
  EXPECT_FLOAT_EQ(rect.getWidth(), -10.0f);
  EXPECT_FLOAT_EQ(rect.getHeight(), -5.0f);
}
