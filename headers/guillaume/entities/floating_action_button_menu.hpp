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

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/floating_action_button_menu.hpp"
#include "guillaume/components/layout.hpp"

#include "guillaume/entities/button_base.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Floating action button menu (speed dial) entity. A trigger
	 * `FloatingActionButton` that expands a column of smaller action buttons
	 * when opened.
	 *
	 * The class is a facade: its state lives in
	 * components::FloatingActionButtonMenu and its rules in
	 * systems::FloatingActionButtonMenu.
	 */
	class FloatingActionButtonMenu:
		public std::enable_shared_from_this<FloatingActionButtonMenu>,
		public ecs::ParentEntityFiller<components::Transform, components::Bound,
									   components::Layout,
									   components::FloatingActionButtonMenu>
	{
		public:
		/// Describes a single action in the menu.
		using Action = components::FloatingActionButtonMenu::Action;

		/**
		 * @brief Builder used to configure and create
		 * `FloatingActionButtonMenu` entities.
		 */
		class Builder: public EntityBuilderBase<FloatingActionButtonMenu>
		{
			private:
			std::string _iconGlyphName;			///< Trigger icon glyph name
			std::vector<Action> _actions;		///< Menu actions
			std::string _accessibilityLabel;	///< Trigger accessibility label

			public:
			/**
			 * @brief Construct a new FloatingActionButtonMenu Builder object.
			 * @param componentRegistry The component registry used to build
			 * entities.
			 * @param entityRegistry The entity registry used to build entities.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the FloatingActionButtonMenu entity from the current
			 * configuration.
			 * @return The newly created FloatingActionButtonMenu.
			 */
			std::shared_ptr<FloatingActionButtonMenu>
				buildEntity(void) override;

			/**
			 * @brief Reset the builder to its initial state.
			 */
			void reset(void) override;

			/**
			 * @brief Set the trigger icon glyph name.
			 * @param iconGlyphName The trigger icon glyph name.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyphName);

			/**
			 * @brief Add an action to the menu.
			 * @param iconGlyphName The action icon glyph name.
			 * @param onClick The action click handler.
			 * @return Reference to the builder for chaining.
			 */
			Builder &addAction(const std::string &iconGlyphName,
							   std::function<void(void)> onClick);

			/**
			 * @brief Add a labelled action (Paper FAB.Group `label`).
			 * @param iconGlyphName Action icon glyph.
			 * @param label Visible and accessible label.
			 * @param onClick Action handler.
			 * @return Reference to this builder for chaining.
			 */
			Builder &addAction(const std::string &iconGlyphName,
							   const std::string &label,
							   std::function<void(void)> onClick);

			/**
			 * @brief Set the trigger accessibility label.
			 * @param accessibilityLabel The trigger accessibility label.
			 * @return Reference to the builder for chaining.
			 */
			Builder &
				withAccessibilityLabel(const std::string &accessibilityLabel);

			/**
			 * @brief Create a FAB menu using the builder.
			 * @param parent The parent entity to which the new entity will be
			 * attached.
			 * @param iconGlyphName The trigger icon glyph name.
			 * @return A shared pointer to the newly created menu entity.
			 */
			std::shared_ptr<FloatingActionButtonMenu>
				makeFloatingActionButtonMenu(std::shared_ptr<Entity> parent,
											 const std::string &iconGlyphName);
		};

		/**
		 * @brief Create the entity, fully built.
		 *
		 * Constructors are private: an entity must be owned before it can
		 * parent its children, so construction and child creation happen
		 * together here.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments.
		 * @return The new entity.
		 */
		template<typename... Arguments>
		static std::shared_ptr<FloatingActionButtonMenu>
			create(Arguments &&...arguments)
		{
			std::shared_ptr<FloatingActionButtonMenu> entity(
				new FloatingActionButtonMenu(
					std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~FloatingActionButtonMenu(void);

		/**
		 * @brief Open the menu.
		 * @return Reference to this menu for chaining.
		 */
		FloatingActionButtonMenu &open(void);

		/**
		 * @brief Close the menu.
		 * @return Reference to this menu for chaining.
		 */
		FloatingActionButtonMenu &close(void);

		/**
		 * @brief Toggle the open state.
		 * @return Reference to this menu for chaining.
		 */
		FloatingActionButtonMenu &toggle(void);

		/**
		 * @brief Get whether the menu is open.
		 * @return True when open, false otherwise.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Set the handler called when the menu opens or closes (Paper
		 * `onStateChange`).
		 * @param onStateChange Handler receiving the new open state.
		 * @return Reference to this menu for chaining.
		 */
		FloatingActionButtonMenu &
			setOnStateChange(std::function<void(bool)> onStateChange);

		/**
		 * @brief Get the number of actions.
		 * @return The number of actions.
		 */
		std::size_t getActionCount(void) const;

		/**
		 * @brief Get the identifier of the trigger child entity.
		 * @return The trigger entity identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getTriggerIdentifier(void) const;

		/**
		 * @brief Get the identifier of an action child entity.
		 * @param index The index of the action.
		 * @return The action entity identifier, or InvalidIdentifier when out
		 * of range.
		 */
		ecs::Entity::Identifier getActionIdentifier(std::size_t index) const;

		private:
		/**
		 * @brief Access the menu state.
		 * @return The components::FloatingActionButtonMenu of this entity.
		 */
		components::FloatingActionButtonMenu &state(void);

		/**
		 * @brief Access the menu state.
		 * @return The components::FloatingActionButtonMenu of this entity.
		 */
		const components::FloatingActionButtonMenu &state(void) const;

		private:
		/**
		 * @brief Construct a FloatingActionButtonMenu entity.
		 * @param registry Reference to the component registry.
		 * @param iconGlyphName Trigger icon glyph name.
		 * @param actions The menu actions.
		 * @param accessibilityLabel Trigger accessibility label.
		 */
		FloatingActionButtonMenu(ecs::ComponentRegistry &registry,
								 const std::string &iconGlyphName,
								 const std::vector<Action> &actions,
								 const std::string &accessibilityLabel);

		/**
		 * @brief Create the child entities.
		 *
		 * Called once by create(), when the entity is owned and can be given
		 * as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
