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

#include "systems/test_rectangle_render.hpp"

#include <cmath>
#include <cstddef>
#include <vector>

#include "guillaume/components/style_state.hpp"
#include "guillaume/systems/focus_ring_render.hpp"
#include "guillaume/theme.hpp"

namespace guillaume::systems::tests
{

	TEST_F(TestRectangleRender, NoStrokeEmitsOnlyTheFill)
	{
		const auto entity = createRectangle();

		_system->update(entity);

		const auto renderable = lastRenderable();
		ASSERT_NE(renderable, nullptr);
		EXPECT_EQ(renderable->getMeshes().size(), 1u);
	}

	TEST_F(TestRectangleRender, ColorWithoutWidthPaintsNoStroke)
	{
		const auto entity = createRectangle();
		_componentRegistry.getComponent<components::Borders>(entity).setColor(
			utility::graphic::Color32Bit(255, 0, 0, 255));

		_system->update(entity);

		EXPECT_EQ(lastRenderable()->getMeshes().size(), 1u);
	}

	TEST_F(TestRectangleRender, StrokeEmitsBandInStrokeColor)
	{
		const auto entity = createRectangle();
		_componentRegistry.getComponent<components::Borders>(entity)
			.setBorderRadius(8.0f)
			.setColor(utility::graphic::Color32Bit(255, 0, 0, 255))
			.setWidth(2.0f);

		_system->update(entity);

		const auto renderable = lastRenderable();
		ASSERT_NE(renderable, nullptr);
		ASSERT_EQ(renderable->getMeshes().size(), 2u);

		const auto stroke = renderable->getMeshes()[1];
		// 4 corners x (16 + 1) samples, each paired outer/inner.
		EXPECT_EQ(stroke->getVertices().size(), 4u * 17u * 2u);
		EXPECT_EQ(stroke->getIndices().size(), 4u * 17u * 6u);
		for (const auto &vertex: stroke->getVertices()) {
			EXPECT_EQ(vertex.getColor().getRed(), 255);
			EXPECT_EQ(vertex.getColor().getGreen(), 0);
		}
	}

	TEST_F(TestRectangleRender, StrokeBandHasRequestedWidth)
	{
		const auto entity = createRectangle();
		_componentRegistry.getComponent<components::Borders>(entity)
			.setColor(utility::graphic::Color32Bit(0, 0, 255, 255))
			.setWidth(3.0f);

		_system->update(entity);

		const auto stroke	= lastRenderable()->getMeshes()[1];
		const auto vertices = stroke->getVertices();
		const auto outer	= vertices[0].getPosition();
		const auto inner	= vertices[1].getPosition();
		// The first sample is the top-right corner; with no radius the inner
		// corner is inset by the width on both axes.
		EXPECT_NEAR(std::abs(outer[0] - inner[0]), 3.0f, 1e-4f);
		EXPECT_NEAR(std::abs(outer[1] - inner[1]), 3.0f, 1e-4f);
	}

	namespace
	{
		bool hasVertexAt(const std::vector<utility::graphic::VertexF> &vertices,
						 float x, float y, std::size_t first = 0,
						 std::size_t step = 1)
		{
			for (std::size_t i = first; i < vertices.size(); i += step) {
				const auto position = vertices[i].getPosition();
				if (std::abs(position[0] - x) < 1e-3f
					&& std::abs(position[1] - y) < 1e-3f) {
					return true;
				}
			}
			return false;
		}
	}	 // namespace

	TEST_F(TestRectangleRender, FillKeepsEachCornerRadius)
	{
		const auto entity = createRectangle();
		_componentRegistry.getComponent<components::Borders>(entity)
			.setTopLeftRadius(20.0f);

		_system->update(entity);

		const auto fill		= lastRenderable()->getMeshes()[0];
		const auto vertices = fill->getVertices();
		// Center, one rounded corner (16 + 1 samples) and three square ones.
		EXPECT_EQ(vertices.size(), 1u + 17u + 3u);
		// The rectangle spans x 0..100 and y 0..-50: the three square
		// corners are exact, the rounded top-left one is cut.
		EXPECT_TRUE(hasVertexAt(vertices, 100.0f, 0.0f));
		EXPECT_TRUE(hasVertexAt(vertices, 100.0f, -50.0f));
		EXPECT_TRUE(hasVertexAt(vertices, 0.0f, -50.0f));
		EXPECT_FALSE(hasVertexAt(vertices, 0.0f, 0.0f));
		EXPECT_TRUE(hasVertexAt(vertices, 20.0f, 0.0f));
		EXPECT_TRUE(hasVertexAt(vertices, 0.0f, -20.0f));
	}

	TEST_F(TestRectangleRender, StrokeKeepsEachCornerRadius)
	{
		const auto entity = createRectangle();
		_componentRegistry.getComponent<components::Borders>(entity)
			.setTopLeftRadius(20.0f)
			.setColor(utility::graphic::Color32Bit(255, 0, 0, 255))
			.setWidth(2.0f);

		_system->update(entity);

		ASSERT_EQ(lastRenderable()->getMeshes().size(), 2u);
		const auto stroke	= lastRenderable()->getMeshes()[1];
		const auto vertices = stroke->getVertices();
		// Outer vertices are the even ones.
		EXPECT_TRUE(hasVertexAt(vertices, 100.0f, -50.0f, 0, 2));
		EXPECT_TRUE(hasVertexAt(vertices, 0.0f, -50.0f, 0, 2));
		EXPECT_FALSE(hasVertexAt(vertices, 0.0f, 0.0f, 0, 2));
		EXPECT_TRUE(hasVertexAt(vertices, 20.0f, 0.0f, 0, 2));
	}

	TEST_F(TestRectangleRender, FocusRingKeepsEachCornerRadius)
	{
		const auto entity = createRectangle();
		_componentRegistry.addComponent<components::StyleState>(entity)
			.setFocused(true);
		_componentRegistry.getComponent<components::Borders>(entity)
			.setTopLeftRadius(20.0f);

		FocusRingRender ring(*_engine);
		ring.bindComponentRegistry(_componentRegistry);
		ring.prepare();
		ring.update(entity);
		ring.cleanup();
		ring.unbindComponentRegistry();

		const auto vertices =
			lastRenderable()->getMeshes().front()->getVertices();
		const float grow = theme::FocusRing::Offset + theme::FocusRing::Width;
		// The bottom-right corner is square: an exact outer vertex there.
		EXPECT_TRUE(hasVertexAt(vertices, 100.0f + grow, -50.0f - grow, 0, 2));
		EXPECT_FALSE(hasVertexAt(vertices, -grow, grow, 0, 2));
	}

	TEST_F(TestRectangleRender, RotatedRectangleKeepsItsTopLeftCorner)
	{
		const auto entity	 = createRectangle();
		const float halfTurn = std::sqrt(0.5f);
		_componentRegistry.getComponent<components::Transform>(entity).setPose(
			utility::graphic::PoseF(
				utility::graphic::PositionF(10.0f, 20.0f, 0.0f),
				utility::graphic::OrientationF(0.0f, 0.0f, halfTurn,
											   halfTurn)));

		_system->update(entity);

		const auto vertices =
			lastRenderable()->getMeshes().front()->getVertices();
		// A quarter turn: the 100 dp width runs along world +Y and the
		// 50 dp height along world +X, from the top-left corner (10, 20).
		EXPECT_TRUE(hasVertexAt(vertices, 10.0f, 20.0f));
		EXPECT_TRUE(hasVertexAt(vertices, 10.0f, 120.0f));
		EXPECT_TRUE(hasVertexAt(vertices, 60.0f, 120.0f));
		EXPECT_TRUE(hasVertexAt(vertices, 60.0f, 20.0f));
	}

	TEST_F(TestRectangleRender, TransparentStrokeIsSkipped)
	{
		const auto entity = createRectangle();
		_componentRegistry.getComponent<components::Borders>(entity)
			.setColor(utility::graphic::Color32Bit(0, 0, 0, 0))
			.setWidth(1.0f);

		_system->update(entity);

		EXPECT_EQ(lastRenderable()->getMeshes().size(), 1u);
	}

	TEST_F(TestRectangleRender, NegativeWidthThrows)
	{
		components::Borders borders;
		EXPECT_THROW(borders.setWidth(-1.0f), std::invalid_argument);
	}

	TEST_F(TestRectangleRender, FocusRingDrawsOnlyWhileFocused)
	{
		const auto entity = createRectangle();
		_componentRegistry.addComponent<components::StyleState>(entity);

		FocusRingRender ring(*_engine);
		ring.bindComponentRegistry(_componentRegistry);

		ring.prepare();
		ring.update(entity);
		ring.cleanup();
		EXPECT_EQ(_engineMock->createObjectCallCount, 0u);

		_componentRegistry.getComponent<components::StyleState>(entity)
			.setFocused(true);
		ring.prepare();
		ring.update(entity);
		ring.cleanup();
		EXPECT_EQ(_engineMock->createObjectCallCount, 1u);
		const auto stroke	= lastRenderable()->getMeshes().front();
		const auto vertices = stroke->getVertices();
		ASSERT_FALSE(vertices.empty());
		EXPECT_EQ(
			vertices.front().getColor(),
			getActiveScheme().getColor(SchemeColorRole::Secondary).getColor());

		_componentRegistry.getComponent<components::StyleState>(entity)
			.setFocused(false);
		ring.prepare();
		ring.update(entity);
		ring.cleanup();
		EXPECT_EQ(_engineMock->removeObjectCallCount, 1u);
		ring.unbindComponentRegistry();
	}

}	 // namespace guillaume::systems::tests
