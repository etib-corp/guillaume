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
#include <vector>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"

#include "guillaume/entities/button_base.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Standard button group entity. Lays out a set of `Button` children
	 * in a row separated by a fixed gap. No selection state is managed.
	 */
	class StandardButtonGroup:
		public std::enable_shared_from_this<StandardButtonGroup>,
		public ecs::ParentEntityFiller<components::Transform, components::Bound>
	{
		public:
		/**
		 * @brief Builder used to configure and create `StandardButtonGroup`
		 * entities.
		 */
		class Builder: public ecs::EntityBuilder
		{
			private:
			std::shared_ptr<StandardButtonGroup>
				_group;							 ///< Group entity being built
			std::vector<std::string> _labels;	 ///< Labels of the buttons
			float _gap;							 ///< Gap between the buttons

			public:
			/**
			 * @brief Construct a new StandardButtonGroup Builder object.
			 * @param componentRegistry The component registry used to build
			 * entities.
			 * @param entityRegistry The entity registry used to build entities.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor for the Builder class.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the group entity.
			 * @param parent The parent entity to which the new entity will be
			 * attached.
			 * @return A shared pointer to the newly created group entity.
			 */
			std::shared_ptr<StandardButtonGroup>
				registerEntity(std::shared_ptr<Entity> parent);

			/**
			 * @brief Reset the builder to its initial state.
			 */
			void reset(void) override;

			/**
			 * @brief Set the labels of the buttons.
			 * @param labels The labels of the buttons.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabels(const std::vector<std::string> &labels);

			/**
			 * @brief Set the gap between the buttons.
			 * @param gap The gap between the buttons.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withGap(const float &gap);
		};

		/**
		 * @brief Director that orchestrates `StandardButtonGroup::Builder`.
		 */
		class Director: public ecs::EntityDirector
		{
			public:
			/**
			 * @brief Construct a new Director object.
			 */
			Director(void);

			/**
			 * @brief Default destructor.
			 */
			~Director(void);

			/**
			 * @brief Create a standard button group using the builder.
			 * @param builder The builder used to configure and create the
			 * group.
			 * @param parent The parent entity to which the new entity will be
			 * attached.
			 * @param labels The labels of the buttons.
			 * @return A shared pointer to the newly created group entity.
			 */
			std::shared_ptr<StandardButtonGroup>
				makeStandardButtonGroup(Builder &builder,
										std::shared_ptr<Entity> parent,
										const std::vector<std::string> &labels);
		};

		private:
		std::vector<std::string> _labels {};	///< Labels of the buttons
		float _gap { 8.0f };					///< Gap between the buttons
		std::vector<std::shared_ptr<ButtonBase>>
			_buttons;	 ///< Internal child button entities

		private:
		/**
		 * @brief Lay out the buttons in a row with the configured gap.
		 */
		void layoutButtons(void);

		public:
		/**
		 * @brief Construct a StandardButtonGroup entity.
		 * @param registry Reference to the component registry.
		 * @param labels The labels of the buttons.
		 * @param gap The gap between the buttons.
		 */
		StandardButtonGroup(ecs::ComponentRegistry &registry,
							const std::vector<std::string> &labels,
							const float &gap);

		/**
		 * @brief Default destructor.
		 */
		~StandardButtonGroup(void);

		/**
		 * @brief Get the number of buttons.
		 * @return The number of buttons.
		 */
		std::size_t getButtonCount(void) const;

		/**
		 * @brief Get the identifier of a button child entity.
		 * @param index The index of the button.
		 * @return The button entity identifier, or InvalidIdentifier when out
		 * of range.
		 */
		ecs::Entity::Identifier getButtonIdentifier(std::size_t index) const;

		/**
		 * @brief Initialize the group entity's derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the group entity's derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
