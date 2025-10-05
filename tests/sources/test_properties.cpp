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

#include "test_properties.hpp"
#include "properties.hpp"
#include <gtest/gtest.h>

TEST(PropertiesTest, DefaultConstructor) {
  Properties props;
  EXPECT_TRUE(props.getAttributes().empty());
}

TEST(PropertiesTest, ConstructorWithAttributes) {
  std::map<std::string, std::any> attrs;
  attrs["width"] = 100;
  attrs["height"] = 200;

  Properties props(attrs);

  EXPECT_EQ(props.get<int>("width"), 100);
  EXPECT_EQ(props.get<int>("height"), 200);
}

TEST(PropertiesTest, GetValue) {
  std::map<std::string, std::any> attrs;
  attrs["color"] = std::string("blue");

  Properties props(attrs);

  EXPECT_EQ(props.get<std::string>("color"), "blue");
}

TEST(PropertiesTest, HasKey) {
  std::map<std::string, std::any> attrs;
  attrs["visible"] = true;

  Properties props(attrs);

  EXPECT_TRUE(props.has("visible"));
  EXPECT_FALSE(props.has("nonexistent"));
}

TEST(PropertiesTest, GetNonExistentKeyReturnsDefault) {
  Properties props;
  EXPECT_EQ(props.get<int>("nonexistent"), 0);
  EXPECT_EQ(props.get<std::string>("nonexistent"), "");
}

TEST(PropertiesTest, GetAttributes) {
  std::map<std::string, std::any> attrs;
  attrs["key1"] = 1;
  attrs["key2"] = std::string("value");

  Properties props(attrs);

  const auto &retrievedAttrs = props.getAttributes();
  EXPECT_EQ(retrievedAttrs.size(), 2);
  EXPECT_TRUE(retrievedAttrs.find("key1") != retrievedAttrs.end());
  EXPECT_TRUE(retrievedAttrs.find("key2") != retrievedAttrs.end());
}
