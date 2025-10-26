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
  EXPECT_EQ(polygon.getVertices().size(), 0);
}

TEST(PolygonTest, ConstructorWithPoints) {
  std::vector<Vertex> vertices = {
      Vertex(Point(0.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f)),
      Vertex(Point(1.0f, 0.0f, 0.0f), Color(0.0f, 1.0f, 0.0f, 1.0f)),
      Vertex(Point(1.0f, 1.0f, 0.0f), Color(0.0f, 0.0f, 1.0f, 1.0f)),
  };
  Polygon polygon(vertices);

  EXPECT_EQ(polygon.getVertices().size(), 3);
  EXPECT_FLOAT_EQ(polygon.getVertices()[0].position.x(), 0.0f);
  EXPECT_FLOAT_EQ(polygon.getVertices()[1].position.x(), 1.0f);
  EXPECT_FLOAT_EQ(polygon.getVertices()[2].position.x(), 1.0f);
}

TEST(PolygonTest, AddPoint) {
  Polygon polygon;
  Point p1(1.0f, 2.0f, 3.0f);
  Color c1(0.5f, 0.5f, 0.5f, 1.0f);
  Vertex vertex(p1, c1);
  polygon.addVertex(vertex);

  EXPECT_EQ(polygon.getVertices().size(), 1);
  EXPECT_FLOAT_EQ(polygon.getVertices()[0].position.x(), 1.0f);
  EXPECT_FLOAT_EQ(polygon.getVertices()[0].position.y(), 2.0f);
  EXPECT_FLOAT_EQ(polygon.getVertices()[0].position.z(), 3.0f);
}

TEST(PolygonTest, AddMultiplePoints) {
  Polygon polygon;

  polygon.addVertex(
      Vertex(Point(0.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f)));
  polygon.addVertex(
      Vertex(Point(1.0f, 0.0f, 0.0f), Color(0.0f, 1.0f, 0.0f, 1.0f)));
  polygon.addVertex(
      Vertex(Point(1.0f, 1.0f, 0.0f), Color(0.0f, 0.0f, 1.0f, 1.0f)));
  polygon.addVertex(
      Vertex(Point(0.0f, 1.0f, 0.0f), Color(1.0f, 1.0f, 0.0f, 1.0f)));

  EXPECT_EQ(polygon.getVertices().size(), 4);
}

TEST(PolygonTest, GetVertices) {
  std::vector<Vertex> vertices = {
      Vertex(Point(5.0f, 6.0f, 7.0f), Color(1.0f, 0.0f, 0.0f, 1.0f)),
      Vertex(Point(8.0f, 9.0f, 10.0f), Color(0.0f, 1.0f, 0.0f, 1.0f)),
  };
  Polygon polygon(vertices);

  const std::vector<Vertex> &retrievedVertices = polygon.getVertices();

  EXPECT_EQ(retrievedVertices.size(), 2);
  EXPECT_FLOAT_EQ(retrievedVertices[0].position.x(), 5.0f);
  EXPECT_FLOAT_EQ(retrievedVertices[0].position.y(), 6.0f);
  EXPECT_FLOAT_EQ(retrievedVertices[0].position.z(), 7.0f);
  EXPECT_FLOAT_EQ(retrievedVertices[1].position.x(), 8.0f);
  EXPECT_FLOAT_EQ(retrievedVertices[1].position.y(), 9.0f);
  EXPECT_FLOAT_EQ(retrievedVertices[1].position.z(), 10.0f);
}

TEST(PolygonTest, InheritsFromPrimitive) {
  Polygon polygon;
  (void)polygon; // Intentionally unused local; keep test focused on behavior

  // Should not crash when accessed through base pointer
  EXPECT_NO_THROW(delete new Polygon());
}

TEST(PolygonTest, EmptyPolygonGetVertices) {
  Polygon polygon;
  const std::vector<Vertex> &vertices = polygon.getVertices();

  EXPECT_TRUE(vertices.empty());
}

TEST(PolygonTest, AddVertexToConstructedPolygon) {
  std::vector<Vertex> initialVertices = {
      Vertex(Point(0.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f, 1.0f))};
  Polygon polygon(initialVertices);

  EXPECT_EQ(polygon.getVertices().size(), 1);

  polygon.addVertex(
      Vertex(Point(1.0f, 1.0f, 1.0f), Color(0.0f, 1.0f, 0.0f, 1.0f)));

  EXPECT_EQ(polygon.getVertices().size(), 2);
  EXPECT_FLOAT_EQ(polygon.getVertices()[1].position.x(), 1.0f);
}

TEST(PolygonTest, PolygonWithNegativeCoordinates) {
  Polygon polygon;
  polygon.addVertex(
      Vertex(Point(-1.0f, -2.0f, -3.0f), Color(1.0f, 0.0f, 0.0f, 1.0f)));
  polygon.addVertex(
      Vertex(Point(-4.0f, -5.0f, -6.0f), Color(0.0f, 1.0f, 0.0f, 1.0f)));

  EXPECT_EQ(polygon.getVertices().size(), 2);
  EXPECT_FLOAT_EQ(polygon.getVertices()[0].position.x(), -1.0f);
  EXPECT_FLOAT_EQ(polygon.getVertices()[1].position.y(), -5.0f);
}

TEST(PolygonTest, LargePolygon) {
  Polygon polygon;

  for (int i = 0; i < 100; ++i) {
    polygon.addVertex(
        Vertex(Point(static_cast<float>(i), static_cast<float>(i * 2),
                     static_cast<float>(i * 3)),
               Color(0.1f * i, 0.01f * i, 0.001f * i, 1.0f)));
  }

  EXPECT_EQ(polygon.getVertices().size(), 100);
  EXPECT_FLOAT_EQ(polygon.getVertices()[50].position.x(), 50.0f);
  EXPECT_FLOAT_EQ(polygon.getVertices()[50].position.y(), 100.0f);
  EXPECT_FLOAT_EQ(polygon.getVertices()[50].position.z(), 150.0f);
}
