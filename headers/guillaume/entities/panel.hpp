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

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/node_entity_builder.hpp"
#include "guillaume/ecs/node_entity_filler.hpp"

#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/borders.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Panel component
	 */
	class Panel:
		public ecs::NodeEntityFiller<components::Transform, components::Bound,
									 components::Color, components::Borders>
	{
		public:
		/**
		 * @brief Builder used to configure and create `Panel` entities.
		 */
		class Builder: public ecs::NodeEntityBuilder
		{
			private:
			std::unique_ptr<Panel>
				_panel;	   ///< Unique pointer to the Panel entity being built
			std::string _name;	  ///< Name of the panel to be used

			public:
			/**
			 * @brief Construct a new Panel Builder object.
			 * @param componentRegistry The component registry to register
			 * components to.
			 * @param entityRegistry The entity registry to register entities
			 * to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor for the Panel Builder class.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the panel entity.
			 * @return The entity identifier of the newly created panel entity.
			 */
			ecs::Entity::Identifier registerEntity(void) override;

			/**
			 * @brief Reset the builder to its initial state for creating a new
			 * Panel entity.
			 */
			void reset(void) override;

			/**
			 * @brief Set the name of the panel to be used for the Panel entity.
			 * @param name The name of the panel to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withName(const std::string &name);
		};

		/**
		 * @brief Director that orchestrates `Panel::Builder` to create
		 * preconfigured panel entities.
		 */
		class Director: public ecs::EntityDirector
		{
			public:
			/**
			 * @brief Construct a new Panel Director object.
			 */
			Director(void);

			/**
			 * @brief Default destructor for the Panel Director class.
			 */
			~Director(void);

			/**
			 * @brief Create a default panel entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the default panel.
			 * @param name The name to assign to the default panel.
			 * @return The entity identifier of the newly created panel entity.
			 */
			ecs::Entity::Identifier makeDefaultPanel(Builder &builder,
													 const std::string &name);
		};

		private:
		std::string _name;	  ///< Name of the panel to be used for this
							  ///< Panel entity

		public:
		/**
		 * @brief Default constructor for the Panel component.
		 * @param registry Reference to the component registry for initializing
		 * components.
		 * @param name The name of the panel to be used for this Panel
		 */
		Panel(ecs::ComponentRegistry &registry, const std::string &name);

		/**
		 * @brief Default destructor for the Panel component.
		 */
		~Panel(void);

		/**
		 * @brief Set the name of the panel for this Panel entity.
		 * @param name The new name of the panel to set.
		 * @return Reference to this Panel for chaining.
		 */
		Panel &setName(const std::string &name);
	};

	/**
	 * @brief Concept to ensure a type inherits from Panel
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromPanel = std::is_base_of_v<Panel, Type>;

}	 // namespace guillaume::entities
