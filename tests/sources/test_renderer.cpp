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

#include "test_renderer.hpp"

TEST(RendererTest, DefaultConstructor) {
  EXPECT_NO_THROW(MockRenderer());
}

TEST(RendererTest, DrawTextPrimitive) {
  MockRenderer renderer;
  auto text = std::make_shared<Text>("Hello World");

  renderer.draw(text);

  EXPECT_EQ(renderer.drawTextCallCount, 1);
  EXPECT_EQ(renderer.drawRectangleCallCount, 0);
  EXPECT_EQ(renderer.drawTriangleCallCount, 0);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
  EXPECT_EQ(renderer.lastText, text);
}

TEST(RendererTest, DrawRectanglePrimitive) {
  MockRenderer renderer;
  auto rectangle =
      std::make_shared<Rectangle>(Point(0, 0, 0), 10.0f, 5.0f, Point(0, 0, 0));

  renderer.draw(rectangle);

  EXPECT_EQ(renderer.drawTextCallCount, 0);
  EXPECT_EQ(renderer.drawRectangleCallCount, 1);
  EXPECT_EQ(renderer.drawTriangleCallCount, 0);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
  EXPECT_EQ(renderer.lastRectangle, rectangle);
}

TEST(RendererTest, DrawTrianglePrimitive) {
  MockRenderer renderer;
  auto triangle = std::make_shared<Triangle>(Point(0, 0, 0), Point(1, 0, 0),
                                             Point(0.5, 1, 0));

  renderer.draw(triangle);

  EXPECT_EQ(renderer.drawTextCallCount, 0);
  EXPECT_EQ(renderer.drawRectangleCallCount, 0);
  EXPECT_EQ(renderer.drawTriangleCallCount, 1);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
  EXPECT_EQ(renderer.lastTriangle, triangle);
}

TEST(RendererTest, DrawPolygonPrimitive) {
  MockRenderer renderer;
  std::vector<Point> points = {Point(0, 0, 0), Point(1, 0, 0), Point(1, 1, 0),
                               Point(0, 1, 0)};
  auto polygon = std::make_shared<Polygon>(points);

  renderer.draw(polygon);

  EXPECT_EQ(renderer.drawTextCallCount, 0);
  EXPECT_EQ(renderer.drawRectangleCallCount, 0);
  EXPECT_EQ(renderer.drawTriangleCallCount, 0);
  EXPECT_EQ(renderer.drawPolygonCallCount, 1);
  EXPECT_EQ(renderer.lastPolygon, polygon);
}

TEST(RendererTest, DrawMultiplePrimitives) {
  MockRenderer renderer;
  auto text = std::make_shared<Text>("Test");
  auto rectangle =
      std::make_shared<Rectangle>(Point(0, 0, 0), 5.0f, 5.0f, Point(0, 0, 0));
  auto triangle = std::make_shared<Triangle>(Point(0, 0, 0), Point(1, 0, 0),
                                             Point(0.5, 1, 0));

  renderer.draw(text);
  renderer.draw(rectangle);
  renderer.draw(triangle);

  EXPECT_EQ(renderer.drawTextCallCount, 1);
  EXPECT_EQ(renderer.drawRectangleCallCount, 1);
  EXPECT_EQ(renderer.drawTriangleCallCount, 1);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
}

TEST(RendererTest, DrawNullPrimitive) {
  MockRenderer renderer;
  std::shared_ptr<Primitive> nullPrimitive = nullptr;

  // Should not crash with null pointer
  EXPECT_NO_THROW(renderer.draw(nullPrimitive));

  // No methods should be called
  EXPECT_EQ(renderer.drawTextCallCount, 0);
  EXPECT_EQ(renderer.drawRectangleCallCount, 0);
  EXPECT_EQ(renderer.drawTriangleCallCount, 0);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
}

TEST(RendererTest, DrawUnknownPrimitiveType) {
  MockRenderer renderer;

  // Create a basic Primitive (not a specific subtype)
  auto primitive = std::make_shared<Primitive>();

  renderer.draw(primitive);

  // No specific draw methods should be called for unknown type
  EXPECT_EQ(renderer.drawTextCallCount, 0);
  EXPECT_EQ(renderer.drawRectangleCallCount, 0);
  EXPECT_EQ(renderer.drawTriangleCallCount, 0);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
}

TEST(RendererTest, DrawTextDirectCall) {
  MockRenderer renderer;
  auto text = std::make_shared<Text>("Direct Call");

  renderer.drawText(text);

  EXPECT_EQ(renderer.drawTextCallCount, 1);
  EXPECT_EQ(renderer.lastText, text);
}

TEST(RendererTest, DrawRectangleDirectCall) {
  MockRenderer renderer;
  auto rectangle =
      std::make_shared<Rectangle>(Point(5, 5, 5), 20.0f, 10.0f, Point(0, 0, 0));

  renderer.drawRectangle(rectangle);

  EXPECT_EQ(renderer.drawRectangleCallCount, 1);
  EXPECT_EQ(renderer.lastRectangle, rectangle);
}

TEST(RendererTest, DrawTriangleDirectCall) {
  MockRenderer renderer;
  auto triangle = std::make_shared<Triangle>(Point(0, 0, 0), Point(2, 0, 0),
                                             Point(1, 2, 0));

  renderer.drawTriangle(triangle);

  EXPECT_EQ(renderer.drawTriangleCallCount, 1);
  EXPECT_EQ(renderer.lastTriangle, triangle);
}

TEST(RendererTest, DrawPolygonDirectCall) {
  MockRenderer renderer;
  std::vector<Point> points = {Point(0, 0, 0), Point(2, 0, 0), Point(2, 2, 0),
                               Point(0, 2, 0), Point(-1, 1, 0)};
  auto polygon = std::make_shared<Polygon>(points);

  renderer.drawPolygon(polygon);

  EXPECT_EQ(renderer.drawPolygonCallCount, 1);
  EXPECT_EQ(renderer.lastPolygon, polygon);
}

TEST(RendererTest, TypeDispatchCorrectness) {
  MockRenderer renderer;

  // Create primitives with different content to ensure correct dispatch
  auto text1 = std::make_shared<Text>("First Text");
  auto text2 = std::make_shared<Text>("Second Text");
  auto rect1 =
      std::make_shared<Rectangle>(Point(0, 0, 0), 10.0f, 10.0f, Point(0, 0, 0));
  auto rect2 =
      std::make_shared<Rectangle>(Point(5, 5, 0), 15.0f, 20.0f, Point(0, 0, 0));

  renderer.draw(text1);
  EXPECT_EQ(renderer.lastText->getContent(), "First Text");

  renderer.draw(rect1);
  EXPECT_EQ(renderer.lastRectangle->getWidth(), 10.0f);

  renderer.draw(text2);
  EXPECT_EQ(renderer.lastText->getContent(), "Second Text");

  renderer.draw(rect2);
  EXPECT_EQ(renderer.lastRectangle->getWidth(), 15.0f);

  // Total calls should be correct
  EXPECT_EQ(renderer.drawTextCallCount, 2);
  EXPECT_EQ(renderer.drawRectangleCallCount, 2);
}

// Test for inheritance hierarchy - Rectangle should dispatch to drawRectangle,
// not drawPolygon
TEST(RendererTest, RectangleInheritanceDispatch) {
  MockRenderer renderer;
  auto rectangle =
      std::make_shared<Rectangle>(Point(0, 0, 0), 8.0f, 6.0f, Point(0, 0, 0));

  // Cast to Polygon* to ensure type dispatch works correctly
  std::shared_ptr<Polygon> polygonPtr = rectangle;
  renderer.draw(polygonPtr);

  // Should still call drawRectangle because dynamic_cast checks derived types
  // first
  EXPECT_EQ(renderer.drawRectangleCallCount, 1);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
}

// Test for inheritance hierarchy - Triangle should dispatch to drawTriangle,
// not drawPolygon
TEST(RendererTest, TriangleInheritanceDispatch) {
  MockRenderer renderer;
  auto triangle = std::make_shared<Triangle>(Point(0, 0, 0), Point(3, 0, 0),
                                             Point(1.5, 3, 0));

  // Cast to Polygon* to ensure type dispatch works correctly
  std::shared_ptr<Polygon> polygonPtr = triangle;
  renderer.draw(polygonPtr);

  // Should still call drawTriangle because dynamic_cast checks derived types
  // first
  EXPECT_EQ(renderer.drawTriangleCallCount, 1);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
}
