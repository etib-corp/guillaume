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

#include "systems/test_interaction.hpp"

#include <cmath>

#include <guillaume/components/bound.hpp>
#include <guillaume/components/click.hpp>
#include <guillaume/components/hover.hpp>
#include <guillaume/components/transform.hpp>
#include <guillaume/ecs/component_registry.hpp>
#include <guillaume/ecs/entity_filler.hpp>
#include <guillaume/ecs/entity_registry.hpp>
#include <guillaume/event/event_bus.hpp>

namespace
{
	utility::graphic::OrientationF
		makeOrientationFromEulerRadians(float rollX, float pitchY, float yawZ)
	{
		const float halfRoll  = rollX / 2.0f;
		const float halfPitch = pitchY / 2.0f;
		const float halfYaw	  = yawZ / 2.0f;

		const float sinRoll	 = std::sin(halfRoll);
		const float cosRoll	 = std::cos(halfRoll);
		const float sinPitch = std::sin(halfPitch);
		const float cosPitch = std::cos(halfPitch);
		const float sinYaw	 = std::sin(halfYaw);
		const float cosYaw	 = std::cos(halfYaw);

		return utility::graphic::OrientationF(
			(sinRoll * cosPitch * cosYaw) - (cosRoll * sinPitch * sinYaw),
			(cosRoll * sinPitch * cosYaw) + (sinRoll * cosPitch * sinYaw),
			(cosRoll * cosPitch * sinYaw) - (sinRoll * sinPitch * cosYaw),
			(cosRoll * cosPitch * cosYaw) + (sinRoll * sinPitch * sinYaw));
	}

	class MockRenderer final: public guillaume::Renderer
	{
		public:
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
			const std::vector<utility::graphic::VertexF> &) override
		{
		}

		utility::math::Vector<std::float_t, 2>
			measureText(const utility::graphic::Text &) override
		{
			return { 0.0f, 0.0f };
		}

		void drawText(const utility::graphic::Text &,
					  const utility::graphic::PoseF &) override
		{
		}

		void drawGlyph(const utility::graphic::Glyph &, const utility::graphic::PoseF &) override
		{
		}
	};

	class InteractionEntity final:
		public guillaume::ecs::EntityFiller<
			guillaume::components::Transform, guillaume::components::Bound,
			guillaume::components::Hover, guillaume::components::Click>
	{
		public:
		explicit InteractionEntity(guillaume::ecs::ComponentRegistry &registry)
			: guillaume::ecs::EntityFiller<guillaume::components::Transform,
										   guillaume::components::Bound,
										   guillaume::components::Hover,
										   guillaume::components::Click>(
				  registry)
		{
		}
	};
}	 // namespace

namespace guillaume::systems::tests
{
	TEST_F(TestInteraction, hoverStateFollowsMouseOnRotatedEntity)
	{
		event::EventBus eventBus;
		MockRenderer renderer;
		ecs::ComponentRegistry componentRegistry;
		ecs::EntityRegistry entityRegistry;
		systems::Interaction interaction(eventBus, renderer);

		auto entity = std::make_unique<InteractionEntity>(componentRegistry);
		const auto identifier = entity->getIdentifier();
		auto &transform =
			componentRegistry.getComponent<components::Transform>(identifier);
		auto &bound =
			componentRegistry.getComponent<components::Bound>(identifier);
		auto &hover =
			componentRegistry.getComponent<components::Hover>(identifier);

		transform.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(300.0f, 300.0f, 0.0f),
			makeOrientationFromEulerRadians(
				20.0f * (3.14159265358979323846f / 180.0f),
				15.0f * (3.14159265358979323846f / 180.0f),
				30.0f * (3.14159265358979323846f / 180.0f))));
		bound.setSize({ 136.0f, 136.0f });

		bool entered = false;
		bool exited	 = false;
		hover.setOnHoverHandler([&entered]() {
			entered = true;
		});
		hover.setOnUnhoverHandler([&exited]() {
			exited = true;
		});

		entityRegistry.addEntity(std::move(entity));

		auto motionInside =
			std::make_unique<utility::event::MouseMotionEvent>();
		motionInside->setPosition({ 300.0f, 232.0f });
		eventBus.publish(std::move(motionInside));
		interaction.routine(componentRegistry, entityRegistry);

		EXPECT_TRUE(entered);
		EXPECT_TRUE(hover.isHovered());

		auto motionOutside =
			std::make_unique<utility::event::MouseMotionEvent>();
		motionOutside->setPosition({ 500.0f, 500.0f });
		eventBus.publish(std::move(motionOutside));
		interaction.routine(componentRegistry, entityRegistry);

		EXPECT_TRUE(exited);
		EXPECT_FALSE(hover.isHovered());
	}

	TEST_F(TestInteraction, clickHandlersTriggerOnRotatedEntity)
	{
		event::EventBus eventBus;
		MockRenderer renderer;
		ecs::ComponentRegistry componentRegistry;
		ecs::EntityRegistry entityRegistry;
		systems::Interaction interaction(eventBus, renderer);

		auto entity = std::make_unique<InteractionEntity>(componentRegistry);
		const auto identifier = entity->getIdentifier();
		auto &transform =
			componentRegistry.getComponent<components::Transform>(identifier);
		auto &bound =
			componentRegistry.getComponent<components::Bound>(identifier);
		auto &click =
			componentRegistry.getComponent<components::Click>(identifier);

		transform.setPose(utility::graphic::PoseF(
			utility::graphic::PositionF(300.0f, 300.0f, 0.0f),
			makeOrientationFromEulerRadians(
				20.0f * (3.14159265358979323846f / 180.0f),
				15.0f * (3.14159265358979323846f / 180.0f),
				30.0f * (3.14159265358979323846f / 180.0f))));
		bound.setSize({ 136.0f, 136.0f });

		bool clickCalled   = false;
		bool releaseCalled = false;
		click
			.setOnClickHandler(
				utility::event::MouseButtonEvent::MouseButton::Left,
				[&clickCalled](
					utility::event::MouseMotionEvent::MousePosition) {
					clickCalled = true;
				})
			.setOnReleaseHandler(
				utility::event::MouseButtonEvent::MouseButton::Left,
				[&releaseCalled](
					utility::event::MouseMotionEvent::MousePosition) {
					releaseCalled = true;
				});

		entityRegistry.addEntity(std::move(entity));

		auto pressEvent = std::make_unique<utility::event::MouseButtonEvent>();
		pressEvent->setPosition({ 300.0f, 232.0f });
		pressEvent->setButtonState(
			utility::event::MouseButtonEvent::MouseButton::Left, true);
		eventBus.publish(std::move(pressEvent));
		interaction.routine(componentRegistry, entityRegistry);

		EXPECT_TRUE(clickCalled);

		auto releaseEvent =
			std::make_unique<utility::event::MouseButtonEvent>();
		releaseEvent->setPosition({ 300.0f, 232.0f });
		releaseEvent->setButtonState(
			utility::event::MouseButtonEvent::MouseButton::Left, false);
		eventBus.publish(std::move(releaseEvent));
		interaction.routine(componentRegistry, entityRegistry);

		EXPECT_TRUE(releaseCalled);
	}
}	 // namespace guillaume::systems::tests