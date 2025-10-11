/*
 Copyright (c) 2025 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

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

#include "color.hpp"
#include "test_color.hpp"

TEST(ColorTest, DefaultConstructor) {
  Color c;
  EXPECT_FLOAT_EQ(c.r(), 0.0f);
  EXPECT_FLOAT_EQ(c.g(), 0.0f);
  EXPECT_FLOAT_EQ(c.b(), 0.0f);
  EXPECT_FLOAT_EQ(c.a(), 1.0f);
}

TEST(ColorTest, ParameterizedConstructors) {
  Color rgb(0.25f, 0.5f, 0.75f);
  EXPECT_FLOAT_EQ(rgb.r(), 0.25f);
  EXPECT_FLOAT_EQ(rgb.g(), 0.5f);
  EXPECT_FLOAT_EQ(rgb.b(), 0.75f);
  EXPECT_FLOAT_EQ(rgb.a(), 1.0f);

  Color rgba(0.1f, 0.2f, 0.3f, 0.4f);
  EXPECT_FLOAT_EQ(rgba.r(), 0.1f);
  EXPECT_FLOAT_EQ(rgba.g(), 0.2f);
  EXPECT_FLOAT_EQ(rgba.b(), 0.3f);
  EXPECT_FLOAT_EQ(rgba.a(), 0.4f);
}

TEST(ColorTest, ClampOnConstructionAndSetters) {
  Color c(-1.0f, 2.0f, 0.5f, 3.0f);
  EXPECT_FLOAT_EQ(c.r(), 0.0f);
  EXPECT_FLOAT_EQ(c.g(), 1.0f);
  EXPECT_FLOAT_EQ(c.b(), 0.5f);
  EXPECT_FLOAT_EQ(c.a(), 1.0f);

  c.setR(1.5f);
  c.setG(-0.5f);
  c.setB(0.4f);
  c.setA(-1.0f);
  EXPECT_FLOAT_EQ(c.r(), 1.0f);
  EXPECT_FLOAT_EQ(c.g(), 0.0f);
  EXPECT_FLOAT_EQ(c.b(), 0.4f);
  EXPECT_FLOAT_EQ(c.a(), 0.0f);
}

TEST(ColorTest, WithAlpha) {
  Color base(0.2f, 0.3f, 0.4f);
  Color c = base.withAlpha(0.5f);
  EXPECT_FLOAT_EQ(c.r(), 0.2f);
  EXPECT_FLOAT_EQ(c.g(), 0.3f);
  EXPECT_FLOAT_EQ(c.b(), 0.4f);
  EXPECT_FLOAT_EQ(c.a(), 0.5f);
}

TEST(ColorTest, Lerp) {
  Color a(0.0f, 0.0f, 0.0f, 0.0f);
  Color b(1.0f, 1.0f, 1.0f, 1.0f);
  Color mid = a.lerp(b, 0.5f);
  EXPECT_FLOAT_EQ(mid.r(), 0.5f);
  EXPECT_FLOAT_EQ(mid.g(), 0.5f);
  EXPECT_FLOAT_EQ(mid.b(), 0.5f);
  EXPECT_FLOAT_EQ(mid.a(), 0.5f);
}

TEST(ColorTest, Premultiplied) {
  Color c(0.5f, 0.2f, 0.1f, 0.5f);
  Color p = c.premultiplied();
  EXPECT_FLOAT_EQ(p.r(), 0.25f);
  EXPECT_FLOAT_EQ(p.g(), 0.1f);
  EXPECT_FLOAT_EQ(p.b(), 0.05f);
  EXPECT_FLOAT_EQ(p.a(), 0.5f);
}

TEST(ColorTest, OverCompositing) {
  Color src(1.0f, 0.0f, 0.0f, 0.5f);
  Color dst(0.0f, 0.0f, 1.0f, 1.0f);
  Color out = src.over(dst);
  // outA = 0.5 + 1 * (1 - 0.5) = 1
  // outR = (1*0.5 + 0*1*(1-0.5))/1 = 0.5
  // outG = 0
  // outB = (0*0.5 + 1*1*(1-0.5))/1 = 0.5
  EXPECT_FLOAT_EQ(out.a(), 1.0f);
  EXPECT_NEAR(out.r(), 0.5f, 1e-6f);
  EXPECT_NEAR(out.g(), 0.0f, 1e-6f);
  EXPECT_NEAR(out.b(), 0.5f, 1e-6f);
}

TEST(ColorTest, ToFromRGBA8) {
  Color c = Color::fromRGBA8(128, 64, 255, 0);
  auto p = c.toRGBA8();
  EXPECT_NEAR(c.r(), 128.0f/255.0f, 1e-6f);
  EXPECT_NEAR(c.g(),  64.0f/255.0f, 1e-6f);
  EXPECT_NEAR(c.b(), 255.0f/255.0f, 1e-6f);
  EXPECT_NEAR(c.a(),   0.0f/255.0f, 1e-6f);
  EXPECT_EQ(p.r, 128);
  EXPECT_EQ(p.g,  64);
  EXPECT_EQ(p.b, 255);
  EXPECT_EQ(p.a,   0);
}

TEST(ColorTest, HexConversions) {
  Color c(1.0f, 0.5f, 0.0f, 1.0f);
  std::string hex = c.toHex();          // alpha omitted when opaque by default
  EXPECT_EQ(hex, "#FF8000");

  std::string hexA = c.toHex(true);     // include alpha
  EXPECT_EQ(hexA, "#FF8000FF");

  Color c2 = Color::fromHex("#FF8000");
  EXPECT_NEAR(c2.r(), 1.0f, 1e-6f);
  EXPECT_NEAR(c2.g(), 128.0f/255.0f, 1e-6f);
  EXPECT_NEAR(c2.b(), 0.0f, 1e-6f);
  EXPECT_NEAR(c2.a(), 1.0f, 1e-6f);

  Color c3 = Color::fromHex("#F80");
  EXPECT_NEAR(c3.r(), 1.0f, 1e-6f);
  EXPECT_NEAR(c3.g(), 0.53333336f, 1e-5f); // 0x88/255
  EXPECT_NEAR(c3.b(), 0.0f, 1e-6f);
  EXPECT_NEAR(c3.a(), 1.0f, 1e-6f);

  Color c4 = Color::fromHex("F80C"); // short RGBA
  EXPECT_NEAR(c4.r(), 1.0f, 1e-6f);
  EXPECT_NEAR(c4.g(), 0.53333336f, 1e-5f);
  EXPECT_NEAR(c4.b(), 0.0f, 1e-6f);
  EXPECT_NEAR(c4.a(), 0.8f, 1e-2f); // 0xCC/255 ~= 0.8
}

TEST(ColorTest, HexInvalidInputs) {
  EXPECT_THROW(Color::fromHex(""), std::invalid_argument);
  EXPECT_THROW(Color::fromHex("#"), std::invalid_argument);
  EXPECT_THROW(Color::fromHex("GGG"), std::invalid_argument);
  EXPECT_THROW(Color::fromHex("12345"), std::invalid_argument);
  EXPECT_THROW(Color::fromHex("#12345Z"), std::invalid_argument);
}

TEST(ColorTest, Operators) {
  Color a(0.3f, 0.3f, 0.3f, 0.3f);
  Color b(0.8f, 0.1f, 0.9f, 0.9f);

  Color add = a + b;
  EXPECT_NEAR(add.r(), 1.0f, 1e-6f);
  EXPECT_NEAR(add.g(), 0.4f, 1e-6f);
  EXPECT_NEAR(add.b(), 1.0f, 1e-6f);
  EXPECT_NEAR(add.a(), 1.0f, 1e-6f);

  Color sub = b - a;
  EXPECT_NEAR(sub.r(), 0.5f, 1e-6f);
  EXPECT_NEAR(sub.g(), 0.0f, 1e-6f);
  EXPECT_NEAR(sub.b(), 0.6f, 1e-6f);
  EXPECT_NEAR(sub.a(), 0.6f, 1e-6f);

  Color scaled = a * 2.0f;
  EXPECT_NEAR(scaled.r(), 0.6f, 1e-6f);
  EXPECT_NEAR(scaled.g(), 0.6f, 1e-6f);
  EXPECT_NEAR(scaled.b(), 0.6f, 1e-6f);
  EXPECT_NEAR(scaled.a(), 0.3f, 1e-6f); // alpha unchanged

  EXPECT_TRUE(Color::Black() == Color(0.0f, 0.0f, 0.0f, 1.0f));
  EXPECT_TRUE(Color::Transparent() == Color(0.0f, 0.0f, 0.0f, 0.0f));
  EXPECT_TRUE(Color::White() != Color::Black());
}
