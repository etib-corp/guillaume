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

#include <utility>
#include <memory>
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/accessibility.hpp"
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
								 components::Image, components::Accessibility>
	{
		public:
		/**
		 * @brief Builder used to configure and create `Image` entities.
		 */
		class Builder: public EntityBuilderBase<Image>
		{
			private:
			std::string _texturePath;	 ///< Name of the texture to be used for
										 ///< this Image entity
			std::string _fallbackPath;	  ///< Texture drawn on load failure
			float _width { 0.0f };		  ///< Requested width (0: keep).
			float _height { 0.0f };		  ///< Requested height (0: keep).

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
			 * @brief Build the Image entity from the current configuration.
			 * @return The newly created Image.
			 */
			std::shared_ptr<Image> buildEntity(void) override;

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

			/**
			 * @brief Set the texture drawn when the main one fails to load.
			 * @param fallbackPath Fallback texture path.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withFallbackPath(const std::string &fallbackPath);

			/**
			 * @brief Set the displayed size.
			 * @param width Width in dp.
			 * @param height Height in dp.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withSize(float width, float height);

			/**
			 * @brief Create a default image entity using the builder.
			 * @param parent The parent entity to which the new image entity
			 * will be attached.
			 * @param texturePath The texture name to assign to the created
			 * entity.
			 * @return A shared pointer to the newly created image entity.
			 */
			std::shared_ptr<Image> makeImage(std::shared_ptr<Entity> parent,
											 const std::string &texturePath);
		};

		public:
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
		static std::shared_ptr<Image> create(Arguments &&...arguments)
		{
			std::shared_ptr<Image> entity(
				new Image(std::forward<Arguments>(arguments)...));
			return entity;
		}

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
		 * @brief Get the texture path.
		 * @return The texture path.
		 */
		const std::string &getTexturePath(void) const;

		/**
		 * @brief Set the texture drawn when the main one fails to load.
		 * @param fallbackPath Fallback texture path (empty to disable).
		 * @return Reference to this image for chaining.
		 */
		Image &setFallbackPath(const std::string &fallbackPath);

		/**
		 * @brief Set the displayed size.
		 * @param width Width in dp, must be positive.
		 * @param height Height in dp, must be positive.
		 * @return Reference to this image for chaining.
		 * @throws std::invalid_argument If a dimension is not positive.
		 */
		Image &setSize(float width, float height);

		/**
		 * @brief Set the height from the width and an aspect ratio
		 * (width / height).
		 * @param width Width in dp, must be positive.
		 * @param aspectRatio Width-to-height ratio, must be positive.
		 * @return Reference to this image for chaining.
		 * @throws std::invalid_argument If an argument is not positive.
		 */
		Image &setAspectRatio(float width, float aspectRatio);

		/**
		 * @brief Get the loading state reported by systems::ImageRender.
		 * @return The loading state.
		 */
		components::Image::LoadState getLoadState(void) const;

		/**
		 * @brief Set the accessibility label (role Image).
		 * @param label The label.
		 * @return Reference to this image for chaining.
		 */
		Image &setAccessibilityLabel(const std::string &label);

		private:
		/**
		 * @brief Default constructor for the Image component.
		 * @param registry Reference to the component registry for initializing
		 * components.
		 * @param texturePath The name of the texture to be used for this Image
		 * entity.
		 */
		Image(ecs::ComponentRegistry &registry, const std::string &texturePath);
	};

}	 // namespace guillaume::entities
