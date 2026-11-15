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

#include <memory>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_registry.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Base of every widget `Builder`: turns a configuration into a
	 * widget that is attached to a parent and owned by an entity registry.
	 *
	 * A concrete builder (Button::Builder, Checkbox::Builder, ...) stores
	 * the configuration in its fields, exposes `with...()` setters and
	 * `make...()` shortcuts, and implements two hooks: buildEntity(), which
	 * creates the widget from the fields, and `reset()`, which restores the
	 * default configuration. registerEntity() does the rest.
	 *
	 * @code
	 * Surface::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto surface = builder.withElevation(theme::ElevationLevel::Level2)
	 * 	.registerEntity(nullptr);
	 * @endcode
	 *
	 * One builder can create several widgets in a row: it is back to its
	 * defaults after each registerEntity(), so nothing leaks from one widget
	 * to the next.
	 *
	 * @tparam EntityType The widget class the builder creates.
	 *
	 * @see ecs::EntityBuilder, ecs::EntityRegistry
	 */
	template<typename EntityType> class EntityBuilderBase:
		public ecs::EntityBuilder
	{
		public:
		/**
		 * @brief Construct a builder bound to two registries.
		 * @param componentRegistry The registry that stores the components
		 * of the widgets built.
		 * @param entityRegistry The registry that owns the widgets built: a
		 * scene's registry for a top-level widget, or a parent widget for
		 * its own children.
		 */
		EntityBuilderBase(ecs::ComponentRegistry &componentRegistry,
						  ecs::EntityRegistry &entityRegistry);

		/**
		 * @brief Default destructor.
		 */
		~EntityBuilderBase(void) override = default;

		/**
		 * @brief Build the widget, attach it to its parent, hand it to the
		 * entity registry, then reset the builder to its defaults.
		 *
		 * The `make...()` methods of the concrete builders end with this
		 * call.
		 * @param parent The entity the widget becomes a child of, or nullptr
		 * for a widget without parent.
		 * @return The registered widget, or nullptr when buildEntity()
		 * returned nullptr (nothing is registered then, but the builder is
		 * still reset).
		 */
		std::shared_ptr<EntityType>
			registerEntity(std::shared_ptr<ecs::Entity> parent);

		protected:
		/**
		 * @brief Create the widget from the current configuration.
		 *
		 * Implemented by each concrete builder, usually as a call to the
		 * widget's static `create()`. It must not attach the parent nor
		 * register the widget: registerEntity() does both.
		 *
		 * @return The new widget, not yet attached nor registered.
		 */
		virtual std::shared_ptr<EntityType> buildEntity(void) = 0;
	};

}	 // namespace guillaume::entities

#include "guillaume/entities/builder_base.tpp"
