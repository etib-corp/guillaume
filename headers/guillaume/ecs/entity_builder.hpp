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
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_registry.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Base class of the widget builders: remembers in which registries
	 * the entities it creates go.
	 *
	 * A builder is the comfortable way to create a widget in a scene: it is
	 * configured with fluent calls or a `makeXxx()` preset, creates the
	 * entity with its components in the component registry, and registers
	 * it in the entity registry. Each widget has its own builder
	 * (`entities::Button::Builder`, ...), derived from
	 * entities::EntityBuilderBase, which adds the creation step. A Scene
	 * holds one builder per widget type in its EntityBuilderManager.
	 *
	 * This base class only stores the two registries and declares reset().
	 * Both registries must outlive the builder.
	 *
	 * @see EntityBuilderManager
	 * @see guillaume::entities::EntityBuilderBase
	 */
	class EntityBuilder
	{
		private:
		ComponentRegistry
			&_componentRegistry;	///< Where the components of the built
									///< entities are created
		EntityRegistry &_entityRegistry;	///< Where the built entities are
											///< registered

		protected:
		/**
		 * @brief Get the registry in which the builder creates components.
		 * @return Reference to the component registry given to the
		 * constructor.
		 */
		ComponentRegistry &getComponentRegistry(void);

		/**
		 * @brief Get the registry in which the builder registers the
		 * entities it creates.
		 * @return Reference to the entity registry given to the constructor.
		 */
		EntityRegistry &getEntityRegistry(void);

		public:
		/**
		 * @brief Create a builder working on a pair of registries, normally
		 * those of one scene.
		 * @param componentRegistry The registry that receives the components
		 * of the built entities. It must outlive the builder.
		 * @param entityRegistry The registry that owns the built entities.
		 * It must outlive the builder.
		 */
		EntityBuilder(ecs::ComponentRegistry &componentRegistry,
					  ecs::EntityRegistry &entityRegistry);

		/**
		 * @brief Virtual destructor.
		 */
		virtual ~EntityBuilder(void) = default;

		/**
		 * @brief Forget the configuration accumulated for the entity being
		 * built, so the builder is ready for the next one.
		 *
		 * Implemented by each widget builder, which restores its options to
		 * their defaults. entities::EntityBuilderBase::registerEntity calls
		 * it after every build.
		 */
		virtual void reset(void) = 0;
	};

	/**
	 * @brief Constrains a template parameter to a type derived from
	 * EntityBuilder.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromEntityBuilder = std::is_base_of_v<EntityBuilder, Type>;

}	 // namespace guillaume::ecs