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
#include "components/test_mesh_fit.hpp"

namespace guillaume::components::tests
{
	TEST_F(TestMeshFit, WithoutAFitSizeTheModelKeepsItsUnits)
	{
		Mesh mesh;

		EXPECT_FLOAT_EQ(mesh.scaleFor(6.4f, 3.2f), 1.0f);
	}

	TEST_F(TestMeshFit, TheLongestSideTakesTheFitSize)
	{
		Mesh mesh;
		mesh.setFitSize(160.0f);

		EXPECT_FLOAT_EQ(mesh.scaleFor(6.4f, 3.2f), 25.0f);
		EXPECT_FLOAT_EQ(mesh.scaleFor(2.0f, 8.0f), 20.0f);
	}

	TEST_F(TestMeshFit, AnEmptyModelIsNotScaled)
	{
		Mesh mesh;
		mesh.setFitSize(160.0f);

		EXPECT_FLOAT_EQ(mesh.scaleFor(0.0f, 0.0f), 1.0f);
	}

	TEST_F(TestMeshFit, SettingTheSameFitSizeDoesNotMarkChanged)
	{
		Mesh mesh;
		mesh.setFitSize(160.0f);
		mesh.setHasChanged(false);

		mesh.setFitSize(160.0f);

		EXPECT_FALSE(mesh.hasChanged());
	}

}	 // namespace guillaume::components::tests
