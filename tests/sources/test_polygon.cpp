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

#include "primitives/polygon.hpp"
#include "test_polygon.hpp"

TEST(PolygonTest, DefaultConstructor) {
  Polygon polygon;
  EXPECT_EQ(polygon.getPoints().size(), 0);
}

TEST(PolygonTest, ConstructorWithPoints) {
  std::vector<Point> points = {Point(0.0f, 0.0f, 0.0f), Point(1.0f, 0.0f, 0.0f),
                               Point(1.0f, 1.0f, 0.0f)};
  Polygon polygon(points);

  EXPECT_EQ(polygon.getPoints().size(), 3);
  EXPECT_FLOAT_EQ(polygon.getPoints()[0].x(), 0.0f);
  EXPECT_FLOAT_EQ(polygon.getPoints()[1].x(), 1.0f);
  EXPECT_FLOAT_EQ(polygon.getPoints()[2].x(), 1.0f);
}

TEST(PolygonTest, AddPoint) {
  Polygon polygon;
  Point p1(1.0f, 2.0f, 3.0f);

  polygon.addPoint(p1);

  EXPECT_EQ(polygon.getPoints().size(), 1);
  EXPECT_FLOAT_EQ(polygon.getPoints()[0].x(), 1.0f);
  EXPECT_FLOAT_EQ(polygon.getPoints()[0].y(), 2.0f);
  EXPECT_FLOAT_EQ(polygon.getPoints()[0].z(), 3.0f);
}

TEST(PolygonTest, AddMultiplePoints) {
  Polygon polygon;

  polygon.addPoint(Point(0.0f, 0.0f, 0.0f));
  polygon.addPoint(Point(1.0f, 0.0f, 0.0f));
  polygon.addPoint(Point(1.0f, 1.0f, 0.0f));
  polygon.addPoint(Point(0.0f, 1.0f, 0.0f));

  EXPECT_EQ(polygon.getPoints().size(), 4);
}

TEST(PolygonTest, GetPoints) {
  std::vector<Point> points = {Point(5.0f, 6.0f, 7.0f), Point(8.0f, 9.0f, 10.0f)};
  Polygon polygon(points);

  const std::vector<Point> &retrievedPoints = polygon.getPoints();

  EXPECT_EQ(retrievedPoints.size(), 2);
  EXPECT_FLOAT_EQ(retrievedPoints[0].x(), 5.0f);
  EXPECT_FLOAT_EQ(retrievedPoints[0].y(), 6.0f);
  EXPECT_FLOAT_EQ(retrievedPoints[0].z(), 7.0f);
  EXPECT_FLOAT_EQ(retrievedPoints[1].x(), 8.0f);
  EXPECT_FLOAT_EQ(retrievedPoints[1].y(), 9.0f);
  EXPECT_FLOAT_EQ(retrievedPoints[1].z(), 10.0f);
}

TEST(PolygonTest, InheritsFromPrimitive) {
  Polygon polygon;
  Primitive *primitivePtr = &polygon;

  // Should not crash when accessed through base pointer
  EXPECT_NO_THROW(delete new Polygon());
}

TEST(PolygonTest, EmptyPolygonGetPoints) {
  Polygon polygon;
  const std::vector<Point> &points = polygon.getPoints();

  EXPECT_TRUE(points.empty());
}

TEST(PolygonTest, AddPointToConstructedPolygon) {
  std::vector<Point> initialPoints = {Point(0.0f, 0.0f, 0.0f)};
  Polygon polygon(initialPoints);

  EXPECT_EQ(polygon.getPoints().size(), 1);

  polygon.addPoint(Point(1.0f, 1.0f, 1.0f));

  EXPECT_EQ(polygon.getPoints().size(), 2);
  EXPECT_FLOAT_EQ(polygon.getPoints()[1].x(), 1.0f);
}

TEST(PolygonTest, PolygonWithNegativeCoordinates) {
  Polygon polygon;
  polygon.addPoint(Point(-1.0f, -2.0f, -3.0f));
  polygon.addPoint(Point(-4.0f, -5.0f, -6.0f));

  EXPECT_EQ(polygon.getPoints().size(), 2);
  EXPECT_FLOAT_EQ(polygon.getPoints()[0].x(), -1.0f);
  EXPECT_FLOAT_EQ(polygon.getPoints()[1].y(), -5.0f);
}

TEST(PolygonTest, LargePolygon) {
  Polygon polygon;

  for (int i = 0; i < 100; ++i) {
    polygon.addPoint(Point(static_cast<float>(i), static_cast<float>(i * 2),
                           static_cast<float>(i * 3)));
  }

  EXPECT_EQ(polygon.getPoints().size(), 100);
  EXPECT_FLOAT_EQ(polygon.getPoints()[50].x(), 50.0f);
  EXPECT_FLOAT_EQ(polygon.getPoints()[50].y(), 100.0f);
  EXPECT_FLOAT_EQ(polygon.getPoints()[50].z(), 150.0f);
}
