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

#include "systems/test_surface_render.hpp"

namespace guillaume::systems::tests
{

	TEST_F(TestSurfaceRender, ElevationAddsShadowMesh)
	{
		const auto entity =
			createEntity<components::Elevation>();

		_componentRegistry.getComponent<components::Elevation>(entity)
			.setLayers(2);

		_elevationSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		// Each layer is a triangle fan with one center vertex and 4 * 9 arc
		// vertices, so 2 layers produce 2 * (1 + 36) vertices.
		EXPECT_EQ(lastMeshVertexCount(), static_cast<std::size_t>(2 * (1 + 36)));
	}

	TEST_F(TestSurfaceRender, ElevationSecondUpdateReusesObject)
	{
		const auto entity = createEntity<components::Elevation>();

		_elevationSystem->update(entity);
		_elevationSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		EXPECT_EQ(_engineMock->updateObjectCallCount, 1u);
	}

	TEST_F(TestSurfaceRender, ElevationCleanupRemovesUnusedMesh)
	{
		const auto entity = createEntity<components::Elevation>();

		_elevationSystem->update(entity);
		_elevationSystem->prepare();
		_elevationSystem->cleanup();

		EXPECT_EQ(_engineMock->removeObjectCallCount, 1u);
	}

	TEST_F(TestSurfaceRender, ScrimAddsQuadMesh)
	{
		const auto entity = createEntity<components::Scrim>();

		_scrimSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		EXPECT_EQ(lastMeshVertexCount(), static_cast<std::size_t>(4));
		EXPECT_EQ(lastMeshIndexCount(), static_cast<std::size_t>(6));
	}

	TEST_F(TestSurfaceRender, ScrimSecondUpdateReusesObject)
	{
		const auto entity = createEntity<components::Scrim>();

		_scrimSystem->update(entity);
		_scrimSystem->update(entity);

		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		EXPECT_EQ(_engineMock->updateObjectCallCount, 1u);
	}

	TEST_F(TestSurfaceRender, ClipResolvesBoundDerivedRect)
	{
		const auto entity = createEntity<components::Clip>(
			utility::graphic::PoseF(
				utility::graphic::PositionF(10.0f, 20.0f, 0.0f),
				utility::graphic::OrientationF()),
			100.0f, 50.0f);

		_clipSystem->update(entity);

		const auto &rect =
			_componentRegistry.getComponent<components::Clip>(entity).getRect();

		EXPECT_FLOAT_EQ(rect.x, 10.0f);
		EXPECT_FLOAT_EQ(rect.y, 20.0f);
		EXPECT_FLOAT_EQ(rect.width, 100.0f);
		EXPECT_FLOAT_EQ(rect.height, 50.0f);
	}

	TEST_F(TestSurfaceRender, ClipMarginInsetsRect)
	{
		const auto entity = createEntity<components::Clip>();

		_componentRegistry.getComponent<components::Clip>(entity).setMargin(5.0f);

		_clipSystem->update(entity);

		const auto &rect =
			_componentRegistry.getComponent<components::Clip>(entity).getRect();

		EXPECT_FLOAT_EQ(rect.x, 5.0f);
		EXPECT_FLOAT_EQ(rect.y, 5.0f);
		EXPECT_FLOAT_EQ(rect.width, 90.0f);
		EXPECT_FLOAT_EQ(rect.height, 40.0f);
	}

	TEST_F(TestSurfaceRender, DisabledClipDoesNotResolveRect)
	{
		const auto entity = createEntity<components::Clip>();

		_componentRegistry.getComponent<components::Clip>(entity).setEnabled(
			false);

		_clipSystem->update(entity);

		const auto &rect =
			_componentRegistry.getComponent<components::Clip>(entity).getRect();

		EXPECT_FLOAT_EQ(rect.width, 0.0f);
		EXPECT_FLOAT_EQ(rect.height, 0.0f);
	}

}	 // namespace guillaume::systems::tests
