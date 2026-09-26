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

#include "systems/test_ellipse_render.hpp"

namespace guillaume::systems::tests
{

	TEST_F(TestEllipseRender, Constructor)
	{
		EXPECT_NO_THROW({ auto ellipseSystem = EllipseRender(_engine); });
	}

	TEST_F(TestEllipseRender, UpdateAddsTriangleFanMesh)
	{
		const auto entity =
			createEllipseEntity(utility::graphic::PoseF(), 100.0f, 50.0f, 8);

		_ellipseSystem->update(entity);

		ASSERT_EQ(_engineStub->createObjectCallCount, 1);
		const auto meshes = _engineStub->lastCreated()->getMeshes();
		ASSERT_FALSE(meshes.empty());
		const auto &mesh = *meshes.front();
		// Center vertex + (segments + 1) outline vertices
		EXPECT_EQ(mesh.getVertices().size(), static_cast<std::size_t>(8 + 2));
		// 3 indices per outline segment
		EXPECT_EQ(mesh.getIndices().size(),
				  static_cast<std::size_t>(3 * (8 + 1)));
	}

	TEST_F(TestEllipseRender, ReusesRenderObjectWhenStateIsUnchanged)
	{
		const auto entity = createEllipseEntity();

		_ellipseSystem->update(entity);
		_ellipseSystem->update(entity);

		EXPECT_EQ(_engineStub->createObjectCallCount, 1);
		EXPECT_EQ(_engineStub->updateObjectCallCount, 1);
		EXPECT_EQ(_engineStub->removeObjectCallCount, 0);
	}

	TEST_F(TestEllipseRender, PoseChangeUpdatesRenderObjectInPlace)
	{
		const auto entity = createEllipseEntity();

		_ellipseSystem->update(entity);

		auto &transform =
			_componentRegistry.getComponent<components::Transform>(entity);
		transform.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(10.0f, 20.0f, 0.0f),
			utility::graphic::OrientationF()));

		_ellipseSystem->update(entity);

		EXPECT_EQ(_engineStub->createObjectCallCount, 1);
		EXPECT_EQ(_engineStub->updateObjectCallCount, 1);
	}

	TEST_F(TestEllipseRender, SegmentChangeUpdatesRenderObjectInPlace)
	{
		const auto entity = createEllipseEntity();

		_ellipseSystem->update(entity);

		auto &ellipse =
			_componentRegistry.getComponent<components::Ellipse>(entity);
		ellipse.setSegments(32);

		_ellipseSystem->update(entity);

		EXPECT_EQ(_engineStub->createObjectCallCount, 1);
		EXPECT_EQ(_engineStub->updateObjectCallCount, 1);
	}

	TEST_F(TestEllipseRender, CenterVertexMatchesEntityCenter)
	{
		const auto entity = createEllipseEntity(
			utility::graphic::PoseF(
				utility::graphic::PositionF(10.0f, 20.0f, 5.0f),
				utility::graphic::OrientationF()),
			100.0f, 50.0f, 8);

		_ellipseSystem->update(entity);

		const auto meshes = _engineStub->lastCreated()->getMeshes();
		ASSERT_FALSE(meshes.empty());
		const auto center = meshes.front()->getVertices().front().getPosition();

		EXPECT_FLOAT_EQ(center.x, 60.0f);
		EXPECT_FLOAT_EQ(center.y, 45.0f);
		EXPECT_FLOAT_EQ(center.z, 5.0f);
	}

	TEST_F(TestEllipseRender, CleanupRemovesUnusedMesh)
	{
		const auto entity = createEllipseEntity();

		_ellipseSystem->update(entity);

		_ellipseSystem->prepare();
		_ellipseSystem->cleanup();

		EXPECT_EQ(_engineStub->removeObjectCallCount, 1);
	}

	TEST_F(TestEllipseRender, CleanupKeepsUsedMesh)
	{
		const auto entity = createEllipseEntity();

		_ellipseSystem->prepare();
		_ellipseSystem->update(entity);
		_ellipseSystem->cleanup();

		EXPECT_EQ(_engineStub->removeObjectCallCount, 0);
		EXPECT_EQ(_engineStub->createObjectCallCount, 1);
	}

}	 // namespace guillaume::systems::tests
