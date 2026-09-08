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

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/borders.hpp"

#include "guillaume/entities/button_base.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Split button entity. A leading action `Button` with a trailing
	 * chevron `IconButton` that exposes a secondary menu trigger. The two parts
	 * share a single outline.
	 */
	class SplitButton:
		public std::enable_shared_from_this<SplitButton>,
		public ecs::ParentEntityFiller<components::Transform, components::Bound>
	{
		public:
		/**
		 * @brief Builder used to configure and create `SplitButton` entities.
		 */
		class Builder: public ecs::EntityBuilder
		{
			private:
			std::shared_ptr<SplitButton>
				_splitButton;			  ///< Split button entity being built
			std::string _labelContent;	  ///< Label content to attach
			std::function<void(void)>
				_onClick;	 ///< Main action click handler
			std::function<void(void)> _onMenuOpen;	  ///< Menu trigger handler

			public:
			/**
			 * @brief Construct a new SplitButton Builder object.
			 * @param componentRegistry The component registry used to build
			 * entities.
			 * @param entityRegistry The entity registry used to build entities.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the split button entity.
			 * @param parent The parent entity to which the new entity will be
			 * attached.
			 * @return A shared pointer to the newly created split button
			 * entity.
			 */
			std::shared_ptr<SplitButton>
				registerEntity(std::shared_ptr<Entity> parent);

			/**
			 * @brief Reset the builder to its initial state.
			 */
			void reset(void) override;

			/**
			 * @brief Set the label content.
			 * @param labelContent The label content to attach.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &labelContent);

			/**
			 * @brief Set the main action click handler.
			 * @param onClick The main action click handler to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnClick(std::function<void(void)> onClick);

			/**
			 * @brief Set the menu trigger handler.
			 * @param onMenuOpen The menu trigger handler to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnMenuOpen(std::function<void(void)> onMenuOpen);
		};

		/**
		 * @brief Director that orchestrates `SplitButton::Builder`.
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
			 * @brief Create a split button entity using the builder.
			 * @param builder The builder used to configure and create the split
			 * button.
			 * @param parent The parent entity to which the new entity will be
			 * attached.
			 * @param labelContent The label content to attach.
			 * @param onClick The main action click handler.
			 * @param onMenuOpen The menu trigger handler.
			 * @return A shared pointer to the newly created split button
			 * entity.
			 */
			std::shared_ptr<SplitButton>
				makeSplitButton(Builder &builder,
								std::shared_ptr<Entity> parent,
								const std::string &labelContent,
								std::function<void(void)> onClick,
								std::function<void(void)> onMenuOpen);
		};

		private:
		std::string _labelContent {};	 ///< Label content to attach.
		std::function<void(void)>
			_onClick {};	///< Main action click handler.
		std::function<void(void)> _onMenuOpen {};	 ///< Menu trigger handler.
		std::shared_ptr<ButtonBase> _action;	///< Leading action child entity
		std::shared_ptr<ButtonBase>
			_chevron;	 ///< Trailing chevron child entity

		private:
		/**
		 * @brief Lay out the leading action and trailing chevron side by side
		 * with connected corners.
		 */
		void layoutParts(void);

		public:
		/**
		 * @brief Construct a SplitButton entity.
		 * @param registry Reference to the component registry.
		 * @param labelContent Label content to attach.
		 * @param onClick Main action click handler.
		 * @param onMenuOpen Menu trigger handler.
		 */
		SplitButton(ecs::ComponentRegistry &registry,
					const std::string &labelContent,
					std::function<void(void)> onClick,
					std::function<void(void)> onMenuOpen);

		/**
		 * @brief Default destructor.
		 */
		~SplitButton(void);

		/**
		 * @brief Get the identifier of the leading action child entity.
		 * @return The action entity identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getActionIdentifier(void) const;

		/**
		 * @brief Get the identifier of the trailing chevron child entity.
		 * @return The chevron entity identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getChevronIdentifier(void) const;

		/**
		 * @brief Initialize the split button entity's derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the split button entity's derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
