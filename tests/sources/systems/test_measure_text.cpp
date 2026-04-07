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

#include <vector>

#include <utility/graphic/text.hpp>

#include "guillaume/components/bound.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/ecs/component_registry.hpp"

#include "systems/test_measure_text.hpp"

namespace
{

	class RendererStub: public guillaume::Renderer
	{
		public:
		utility::math::Vector<std::float_t, 2> measurement = { 0.0f, 0.0f };
		std::size_t measureCallCount					   = 0;
		std::string lastContent;

		ViewportSize getViewportSize(void) const override
		{
			return { 800.0f, 600.0f };
		}
		void clear(void) override
		{
		}
		void present(void) override
		{
		}
		void drawVertices(
			const std::vector<utility::graphic::VertexF> &vertices) override
		{
			(void)vertices;
		}
		utility::math::Vector<std::float_t, 2>
			measureText(const utility::graphic::Text &text) override
		{
			++measureCallCount;
			lastContent = text.getContent();
			return measurement;
		}
		void drawText(const utility::graphic::Text &text, const utility::graphic::PoseF &pose) override
		{
			(void)text;
			(void)pose;
		}
		void drawGlyph(const utility::graphic::Glyph &glyph, const utility::graphic::PoseF &pose) override
		{
			(void)glyph;
			(void)pose;
		}
	};

	class MeasureTextFixture: public guillaume::systems::tests::TestMeasureText
	{
		protected:
		RendererStub renderer;
		guillaume::systems::MeasureText measureTextSystem { renderer };
		guillaume::ecs::ComponentRegistry componentRegistry;
		guillaume::ecs::Entity::Identifier entityIdentifier { 1 };

		void SetUp(void) override
		{
			componentRegistry.addComponent<guillaume::components::Transform>(
				entityIdentifier);
			componentRegistry.addComponent<guillaume::components::Text>(
				entityIdentifier);
			componentRegistry.addComponent<guillaume::components::Bound>(
				entityIdentifier);
		}
	};

}	 // namespace

TEST_F(MeasureTextFixture, SynchronizesBoundSizeWithMeasuredText)
{
	componentRegistry
		.getComponent<guillaume::components::Text>(entityIdentifier)
		.setContent("Measure me")
		.setFontSize(32);
	renderer.measurement = { 140.0f, 28.0f };

	measureTextSystem.updateEntity(componentRegistry, entityIdentifier);

	const auto size =
		componentRegistry
			.getComponent<guillaume::components::Bound>(entityIdentifier)
			.getSize();
	EXPECT_FLOAT_EQ(size[0], 140.0f);
	EXPECT_FLOAT_EQ(size[1], 28.0f);
	EXPECT_EQ(renderer.measureCallCount, 1);
	EXPECT_EQ(renderer.lastContent, "Measure me");
}

TEST_F(MeasureTextFixture, SkipsMeasurementWhenRequiredComponentIsMissing)
{
	componentRegistry.removeComponent<guillaume::components::Bound>(
		entityIdentifier);

	measureTextSystem.updateEntity(componentRegistry, entityIdentifier);

	EXPECT_EQ(renderer.measureCallCount, 0);
}

namespace guillaume::systems::tests
{
}	 // namespace guillaume::systems::tests
