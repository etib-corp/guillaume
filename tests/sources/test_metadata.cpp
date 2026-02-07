/*
 Copyright (c) 2026 ETIB Corporation

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

#include "test_metadata.hpp"

namespace guillaume::tests {

TEST_F(TestMetadata, DefaultStateIsEmpty) {
	Metadata metadata;

	EXPECT_TRUE(metadata.getName().empty());
	EXPECT_TRUE(metadata.getVersion().empty());
	EXPECT_TRUE(metadata.getIdentifier().empty());
	EXPECT_TRUE(metadata.getCreator().empty());
	EXPECT_TRUE(metadata.getCopyright().empty());
	EXPECT_TRUE(metadata.getUrl().empty());
	EXPECT_TRUE(metadata.getType().empty());
}

TEST_F(TestMetadata, ConstructorSetsAllFields) {
	Metadata metadata("App", "1.2.3", "com.etib.app", "ETIB",
					  "Copyright 2026 ETIB", "https://etib.com", "tool");

	EXPECT_EQ(metadata.getName(), "App");
	EXPECT_EQ(metadata.getVersion(), "1.2.3");
	EXPECT_EQ(metadata.getIdentifier(), "com.etib.app");
	EXPECT_EQ(metadata.getCreator(), "ETIB");
	EXPECT_EQ(metadata.getCopyright(), "Copyright 2026 ETIB");
	EXPECT_EQ(metadata.getUrl(), "https://etib.com");
	EXPECT_EQ(metadata.getType(), "tool");
}

TEST_F(TestMetadata, SettersUpdateFields) {
	Metadata metadata;

	metadata.setName("App");
	metadata.setVersion("2.0.0");
	metadata.setIdentifier("com.etib.app");
	metadata.setCreator("ETIB");
	metadata.setCopyright("Copyright 2026 ETIB");
	metadata.setUrl("https://etib.com");
	metadata.setType("tool");

	EXPECT_EQ(metadata.getName(), "App");
	EXPECT_EQ(metadata.getVersion(), "2.0.0");
	EXPECT_EQ(metadata.getIdentifier(), "com.etib.app");
	EXPECT_EQ(metadata.getCreator(), "ETIB");
	EXPECT_EQ(metadata.getCopyright(), "Copyright 2026 ETIB");
	EXPECT_EQ(metadata.getUrl(), "https://etib.com");
	EXPECT_EQ(metadata.getType(), "tool");
}

TEST_F(TestMetadata, GettersExposeMutableReferences) {
	Metadata metadata;

	metadata.getName() = "App";
	metadata.getVersion() = "3.0.0";
	metadata.getIdentifier() = "com.etib.mutable";
	metadata.getCreator() = "ETIB";
	metadata.getCopyright() = "Copyright 2026";
	metadata.getUrl() = "https://mutable.etib.com";
	metadata.getType() = "library";

	EXPECT_EQ(metadata.getName(), "App");
	EXPECT_EQ(metadata.getVersion(), "3.0.0");
	EXPECT_EQ(metadata.getIdentifier(), "com.etib.mutable");
	EXPECT_EQ(metadata.getCreator(), "ETIB");
	EXPECT_EQ(metadata.getCopyright(), "Copyright 2026");
	EXPECT_EQ(metadata.getUrl(), "https://mutable.etib.com");
	EXPECT_EQ(metadata.getType(), "library");
}

} // namespace guillaume::tests