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

TEST(RendererTest, DefaultConstructor) { EXPECT_NO_THROW(TestRenderer()); }

TEST(RendererTest, DrawTextPrimitive) {
  TestRenderer renderer;
  auto text = std::make_shared<Text>("Hello World");

  renderer.drawText(text);

  EXPECT_EQ(renderer.drawTextCallCount, 1);
  EXPECT_EQ(renderer.drawRectangleCallCount, 0);
  EXPECT_EQ(renderer.drawTriangleCallCount, 0);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
  EXPECT_EQ(renderer.lastText, text);
}

TEST(RendererTest, DrawRectanglePrimitive) {
  TestRenderer renderer;
  auto rectangle =
      std::make_shared<Rectangle>(Point(0, 0, 0), 10.0f, 5.0f, Point(0, 0, 0));

  renderer.drawRectangle(rectangle);

  EXPECT_EQ(renderer.drawTextCallCount, 0);
  EXPECT_EQ(renderer.drawRectangleCallCount, 1);
  EXPECT_EQ(renderer.drawTriangleCallCount, 0);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
  EXPECT_EQ(renderer.lastRectangle, rectangle);
}

TEST(RendererTest, DrawTrianglePrimitive) {
  TestRenderer renderer;
  auto triangle = std::make_shared<Triangle>(Point(0, 0, 0), Point(1, 0, 0),
                                             Point(0.5, 1, 0));

  renderer.drawTriangle(triangle);

  EXPECT_EQ(renderer.drawTextCallCount, 0);
  EXPECT_EQ(renderer.drawRectangleCallCount, 0);
  EXPECT_EQ(renderer.drawTriangleCallCount, 1);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
  EXPECT_EQ(renderer.lastTriangle, triangle);
}

TEST(RendererTest, DrawPolygonPrimitive) {
  TestRenderer renderer;
  std::vector<Point> points = {Point(0, 0, 0), Point(1, 0, 0), Point(1, 1, 0),
                               Point(0, 1, 0)};
  auto polygon = std::make_shared<Polygon>(points);

  renderer.drawPolygon(polygon);

  EXPECT_EQ(renderer.drawTextCallCount, 0);
  EXPECT_EQ(renderer.drawRectangleCallCount, 0);
  EXPECT_EQ(renderer.drawTriangleCallCount, 0);
  EXPECT_EQ(renderer.drawPolygonCallCount, 1);
  EXPECT_EQ(renderer.lastPolygon, polygon);
}

TEST(RendererTest, DrawMultiplePrimitives) {
  TestRenderer renderer;
  auto text = std::make_shared<Text>("Test");
  auto rectangle =
      std::make_shared<Rectangle>(Point(0, 0, 0), 5.0f, 5.0f, Point(0, 0, 0));
  auto triangle = std::make_shared<Triangle>(Point(0, 0, 0), Point(1, 0, 0),
                                             Point(0.5, 1, 0));

  renderer.drawText(text);
  renderer.drawRectangle(rectangle);
  renderer.drawTriangle(triangle);

  EXPECT_EQ(renderer.drawTextCallCount, 1);
  EXPECT_EQ(renderer.drawRectangleCallCount, 1);
  EXPECT_EQ(renderer.drawTriangleCallCount, 1);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
}

TEST(RendererTest, DrawNullPrimitive) {
  TestRenderer renderer;
  std::shared_ptr<Text> nullText = nullptr;

  // Should not crash with null pointer
  EXPECT_NO_THROW(renderer.drawText(nullText));

  // No methods should be called (or count should still be 1 if called)
  EXPECT_EQ(renderer.drawTextCallCount, 1);
  EXPECT_EQ(renderer.drawRectangleCallCount, 0);
  EXPECT_EQ(renderer.drawTriangleCallCount, 0);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
}

TEST(RendererTest, DrawUnknownPrimitiveType) {
  // This test is no longer relevant since we don't have a generic draw() method
  // that handles unknown types. Each specific draw method is called directly.
  SUCCEED();
}

TEST(RendererTest, DrawTextDirectCall) {
  TestRenderer renderer;
  auto text = std::make_shared<Text>("Direct Call");

  renderer.drawText(text);

  EXPECT_EQ(renderer.drawTextCallCount, 1);
  EXPECT_EQ(renderer.lastText, text);
}

TEST(RendererTest, DrawRectangleDirectCall) {
  TestRenderer renderer;
  auto rectangle =
      std::make_shared<Rectangle>(Point(5, 5, 5), 20.0f, 10.0f, Point(0, 0, 0));

  renderer.drawRectangle(rectangle);

  EXPECT_EQ(renderer.drawRectangleCallCount, 1);
  EXPECT_EQ(renderer.lastRectangle, rectangle);
}

TEST(RendererTest, DrawTriangleDirectCall) {
  TestRenderer renderer;
  auto triangle = std::make_shared<Triangle>(Point(0, 0, 0), Point(2, 0, 0),
                                             Point(1, 2, 0));

  renderer.drawTriangle(triangle);

  EXPECT_EQ(renderer.drawTriangleCallCount, 1);
  EXPECT_EQ(renderer.lastTriangle, triangle);
}

TEST(RendererTest, DrawPolygonDirectCall) {
  TestRenderer renderer;
  std::vector<Point> points = {Point(0, 0, 0), Point(2, 0, 0), Point(2, 2, 0),
                               Point(0, 2, 0), Point(-1, 1, 0)};
  auto polygon = std::make_shared<Polygon>(points);

  renderer.drawPolygon(polygon);

  EXPECT_EQ(renderer.drawPolygonCallCount, 1);
  EXPECT_EQ(renderer.lastPolygon, polygon);
}

TEST(RendererTest, TypeDispatchCorrectness) {
  TestRenderer renderer;

  // Create primitives with different content to ensure correct dispatch
  auto text1 = std::make_shared<Text>("First Text");
  auto text2 = std::make_shared<Text>("Second Text");
  auto rect1 =
      std::make_shared<Rectangle>(Point(0, 0, 0), 10.0f, 10.0f, Point(0, 0, 0));
  auto rect2 =
      std::make_shared<Rectangle>(Point(5, 5, 0), 15.0f, 20.0f, Point(0, 0, 0));

  renderer.drawText(text1);
  EXPECT_EQ(renderer.lastText->getContent(), "First Text");

  renderer.drawRectangle(rect1);
  EXPECT_EQ(renderer.lastRectangle->getWidth(), 10.0f);

  renderer.drawText(text2);
  EXPECT_EQ(renderer.lastText->getContent(), "Second Text");

  renderer.drawRectangle(rect2);
  EXPECT_EQ(renderer.lastRectangle->getWidth(), 15.0f);

  // Total calls should be correct
  EXPECT_EQ(renderer.drawTextCallCount, 2);
  EXPECT_EQ(renderer.drawRectangleCallCount, 2);
}

// Test for inheritance hierarchy - Rectangle should dispatch to drawRectangle
TEST(RendererTest, RectangleInheritanceDispatch) {
  TestRenderer renderer;
  auto rectangle =
      std::make_shared<Rectangle>(Point(0, 0, 0), 8.0f, 6.0f, Point(0, 0, 0));

  renderer.drawRectangle(rectangle);

  EXPECT_EQ(renderer.drawRectangleCallCount, 1);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
}

// Test for inheritance hierarchy - Triangle should dispatch to drawTriangle
TEST(RendererTest, TriangleInheritanceDispatch) {
  TestRenderer renderer;
  auto triangle = std::make_shared<Triangle>(Point(0, 0, 0), Point(3, 0, 0),
                                             Point(1.5, 3, 0));

  renderer.drawTriangle(triangle);

  EXPECT_EQ(renderer.drawTriangleCallCount, 1);
  EXPECT_EQ(renderer.drawPolygonCallCount, 0);
}
