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

#include "systems/test_text_input.hpp"

#include <memory>

#include "guillaume/components/focus.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_registry.hpp"
#include "guillaume/ecs/entity_registry_container.hpp"
#include "guillaume/ecs/level_order_traveler.hpp"
#include "guillaume/event/event_bus.hpp"

#include <utility/event/text_input_event.hpp>

namespace
{

	class TextInputFixture:
		public guillaume::systems::tests::TestSystemsTextInput
	{
		protected:
		guillaume::event::EventBus eventBus;
		guillaume::systems::TextInput textInputSystem { eventBus };
		guillaume::ecs::ComponentRegistry componentRegistry;
		guillaume::ecs::EntityRegistryContainer entityRegistry;
		guillaume::ecs::LevelOrderTraveler traveler;
		guillaume::ecs::Entity::Identifier entityIdentifier {
			guillaume::ecs::Entity::InvalidIdentifier
		};

		void SetUp(void) override
		{
			auto entity		 = std::make_unique<guillaume::ecs::Entity>();
			entityIdentifier = entity->getIdentifier();
			entity->setSignature(guillaume::ecs::Entity::getSignatureFromTypes<
								 guillaume::components::Text,
								 guillaume::components::Focus>());
			entityRegistry.addEntity(std::move(entity));

			componentRegistry.addComponent<guillaume::components::Text>(
				entityIdentifier);
			componentRegistry
				.addComponent<guillaume::components::Focus>(entityIdentifier)
				.setFocused(true);
		}

		void runFrameWithoutEvent(void)
		{
			textInputSystem.routine(componentRegistry, entityRegistry,
									traveler);
		}

		guillaume::ecs::Entity::Identifier addTextEntity(bool focused)
		{
			auto entity			  = std::make_unique<guillaume::ecs::Entity>();
			const auto identifier = entity->getIdentifier();
			entity->setSignature(guillaume::ecs::Entity::getSignatureFromTypes<
								 guillaume::components::Text,
								 guillaume::components::Focus>());
			entityRegistry.addEntity(std::move(entity));

			componentRegistry.addComponent<guillaume::components::Text>(
				identifier);
			componentRegistry
				.addComponent<guillaume::components::Focus>(identifier)
				.setFocused(focused);
			return identifier;
		}

		void dispatchTextInputEvent(const std::string &textInput)
		{
			auto event = std::make_shared<utility::event::TextInputEvent>();
			event->setText(textInput);
			eventBus.publish(std::move(event));
			textInputSystem.routine(componentRegistry, entityRegistry,
									traveler);
		}

		std::string getContent(void) const
		{
			return componentRegistry
				.getComponent<guillaume::components::Text>(entityIdentifier)
				.getContent();
		}
	};

}	 // namespace

TEST_F(TextInputFixture, AppendsCommittedTextInput)
{
	dispatchTextInputEvent("a");
	dispatchTextInputEvent("A");
	dispatchTextInputEvent("7");

	EXPECT_EQ(getContent(), "aA7");
}

TEST_F(TextInputFixture, AppendsPunctuationAndSymbolsFromTextInput)
{
	dispatchTextInputEvent(".,;:?!'\"");
	dispatchTextInputEvent("()[]{}-_+=/\\*@#$%&~^`");

	EXPECT_EQ(getContent(), ".,;:?!'\"()[]{}-_+=/\\*@#$%&~^`");
}

// #176: an event is applied on the frame it is consumed, not on the next
// ones.
TEST_F(TextInputFixture, TextIsNotAppliedAgainOnFramesWithoutEvent)
{
	dispatchTextInputEvent("a");
	runFrameWithoutEvent();
	runFrameWithoutEvent();

	EXPECT_EQ(getContent(), "a");
}

// #177: only the focused entity receives the text.
TEST_F(TextInputFixture, OnlyTheFocusedEntityReceivesText)
{
	const auto other = addTextEntity(false);

	dispatchTextInputEvent("a");

	EXPECT_EQ(getContent(), "a");
	EXPECT_TRUE(
		componentRegistry.getComponent<guillaume::components::Text>(other)
			.getContent()
			.empty());
}

// #177: the text goes where the cursor is.
TEST_F(TextInputFixture, InsertsAtTheCursor)
{
	auto &text = componentRegistry.getComponent<guillaume::components::Text>(
		entityIdentifier);
	text.setContent("ad");
	text.setCursorPosition(1);

	dispatchTextInputEvent("bc");

	EXPECT_EQ(getContent(), "abcd");
	EXPECT_EQ(text.getCursorPosition(), 3u);
}

namespace guillaume::systems::tests
{
}	 // namespace guillaume::systems::tests
