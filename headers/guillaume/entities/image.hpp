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

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/image.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Image component
	 */
	class Image:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Image>
	{
		public:
		/**
		 * @brief Builder used to configure and create `Image` entities.
		 */
		class Builder: public ecs::EntityBuilder
		{
			private:
			std::shared_ptr<Image>
				_image;	   ///< Unique pointer to the Image entity being built
			std::string _texturePath;	 ///< Name of the texture to be used for
										 ///< this Image entity

			public:
			/**
			 * @brief Construct a new Image Builder object.
			 * @param componentRegistry The component registry to register
			 * components to.
			 * @param entityRegistry The entity registry to register entities
			 * to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor for the Image Builder class.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the image entity.
			 * @param parent The parent entity to which the new image entity
			 * will be attached.
			 * @return A shared pointer to the newly created image entity.
			 */
			std::shared_ptr<Image>
				registerEntity(std::shared_ptr<Entity> parent);

			/**
			 * @brief Reset the builder to its initial state for creating a new
			 * Image entity.
			 */
			void reset(void) override;

			/**
			 * @brief Set the name of the texture to be used for the Image
			 * entity.
			 * @param texturePath The texture identifier to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTexturePath(const std::string &texturePath);
		};

		/**
		 * @brief Director that orchestrates `Image::Builder` to create
		 * preconfigured image entities.
		 */
		class Director: public ecs::EntityDirector
		{
			public:
			/**
			 * @brief Construct a new Image Director object.
			 */
			Director(void);

			/**
			 * @brief Default destructor for the Image Director class.
			 */
			~Director(void);

			/**
			 * @brief Create a default image entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the default image.
			 * @param parent The parent entity to which the new image entity
			 * will be attached.
			 * @param texturePath The texture name to assign to the created
			 * entity.
			 * @return A shared pointer to the newly created image entity.
			 */
			std::shared_ptr<Image> makeImage(Builder &builder,
											 std::shared_ptr<Entity> parent,
											 const std::string &texturePath);
		};

		private:
		std::string _texturePath;	 ///< Name of the texture to be used for
									 ///< this Image entity

		public:
		/**
		 * @brief Default constructor for the Image component.
		 * @param registry Reference to the component registry for initializing
		 * components.
		 * @param texturePath The name of the texture to be used for this Image
		 * entity.
		 */
		Image(ecs::ComponentRegistry &registry, const std::string &texturePath);

		/**
		 * @brief Default destructor for the Image component.
		 */
		~Image(void);

		/**
		 * @brief Set the name of the texture for this Image entity.
		 * @param texturePath The new name of the texture to set.
		 * @return Reference to this Image for chaining.
		 */
		Image &setTexturePath(const std::string &texturePath);

		/**
		 * @brief Initialize the image entity's derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the image entity's derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
