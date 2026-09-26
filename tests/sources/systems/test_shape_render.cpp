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

#include "systems/test_shape_render.hpp"

namespace guillaume::systems::tests
{

	TEST_F(TestShapeRender, RingUpdateAddsAnnulusMesh)
	{
		const auto entity = createShapeEntity<components::Ring>();

		_componentRegistry.getComponent<components::Ring>(entity)
			.setThickness(8.0f)
			.setSegments(8);

		_ringSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		// 2 vertices per segment (outer + inner).
		EXPECT_EQ(lastMeshVertexCount(), static_cast<std::size_t>(2 * 8));
		// 2 triangles per segment.
		EXPECT_EQ(lastMeshIndexCount(), static_cast<std::size_t>(6 * 8));
	}

	TEST_F(TestShapeRender, RingSecondUpdateReusesObject)
	{
		const auto entity = createShapeEntity<components::Ring>();

		_ringSystem->update(entity);
		_ringSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		EXPECT_EQ(_engineMock->updateObjectCallCount, 1u);
	}

	TEST_F(TestShapeRender, RingThicknessChangeUpdatesMesh)
	{
		const auto entity = createShapeEntity<components::Ring>();

		_ringSystem->update(entity);

		_componentRegistry.getComponent<components::Ring>(entity)
			.setThickness(12.0f);

		_ringSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		EXPECT_EQ(_engineMock->updateObjectCallCount, 1u);
	}

	TEST_F(TestShapeRender, RingCleanupRemovesUnusedMesh)
	{
		const auto entity = createShapeEntity<components::Ring>();

		_ringSystem->update(entity);
		_ringSystem->prepare();
		_ringSystem->cleanup();

		EXPECT_EQ(_engineMock->removeObjectCallCount, 1u);
	}

	TEST_F(TestShapeRender, ArcUpdateAddsOpenStripMesh)
	{
		const auto entity = createShapeEntity<components::Arc>();

		_componentRegistry.getComponent<components::Arc>(entity)
			.setSegments(8)
			.setStartAngle(0.0f)
			.setSweepAngle(3.14159265f);

		_arcSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		// 2 vertices per sample, one extra sample for the open strip.
		EXPECT_EQ(lastMeshVertexCount(), static_cast<std::size_t>(2 * (8 + 1)));
		EXPECT_EQ(lastMeshIndexCount(), static_cast<std::size_t>(6 * 8));
	}

	TEST_F(TestShapeRender, ArcSecondUpdateReusesObject)
	{
		const auto entity = createShapeEntity<components::Arc>();

		_arcSystem->update(entity);
		_arcSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		EXPECT_EQ(_engineMock->updateObjectCallCount, 1u);
	}

	TEST_F(TestShapeRender, ArcSweepChangeUpdatesMesh)
	{
		const auto entity = createShapeEntity<components::Arc>();

		_arcSystem->update(entity);

		_componentRegistry.getComponent<components::Arc>(entity)
			.setSweepAngle(1.0f);

		_arcSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		EXPECT_EQ(_engineMock->updateObjectCallCount, 1u);
	}

	TEST_F(TestShapeRender, LineUpdateAddsQuadMesh)
	{
		const auto entity = createShapeEntity<components::Line>();

		_lineSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		EXPECT_EQ(lastMeshVertexCount(), static_cast<std::size_t>(4));
		EXPECT_EQ(lastMeshIndexCount(), static_cast<std::size_t>(6));
	}

	TEST_F(TestShapeRender, DashedLineAddsMultipleQuads)
	{
		const auto entity = createShapeEntity<components::Line>(
			utility::graphic::PoseF(), 20.0f, 4.0f);

		_componentRegistry.getComponent<components::Line>(entity)
			.setDashed(true)
			.setDashLength(5.0f)
			.setGapLength(5.0f);

		_lineSystem->update(entity);

		// Half length 10, period 10: two dashes fit ([-10,-5] and [0,5]).
		EXPECT_EQ(lastMeshVertexCount(), static_cast<std::size_t>(8));
		EXPECT_EQ(lastMeshIndexCount(), static_cast<std::size_t>(12));
	}

	TEST_F(TestShapeRender, LineSecondUpdateReusesObject)
	{
		const auto entity = createShapeEntity<components::Line>();

		_lineSystem->update(entity);
		_lineSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		EXPECT_EQ(_engineMock->updateObjectCallCount, 1u);
	}

}	 // namespace guillaume::systems::tests
