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
	 * @brief A picture loaded from a texture file and stretched over a
	 * rectangle.
	 *
	 * Image is a leaf widget: it has no children. Its state lives in
	 * components::Image (texture path, fallback path, loading state),
	 * components::Bound (displayed size), components::Transform and
	 * components::Accessibility (role Image and label). systems::ImageRender
	 * loads the texture and draws it over the Bound each frame, in the
	 * Render phase, and records the outcome as the loading state. The class
	 * itself holds no field.
	 *
	 * @code
	 * Image::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto photo = builder.withSize(160.0f, 90.0f)
	 * 	.withFallbackPath("images/placeholder.png")
	 * 	.makeImage(parent, "images/photo.png");
	 * photo->setAccessibilityLabel("Team photo");
	 * @endcode
	 *
	 * `Image::create(registry, texturePath)` builds the same widget without
	 * registering it in an entity registry; set its size and fallback with
	 * the setters afterwards.
	 *
	 * Watch out:
	 * - No system measures the texture. The picture fills the Bound, so
	 *   give the image a size with Builder::withSize(), setSize() or
	 *   setAspectRatio().
	 * - The texture is stretched to the Bound; the aspect ratio of the file
	 *   is not preserved.
	 * - getLoadState() is `Pending` until the first Render phase.
	 *
	 * @see components::Image, systems::ImageRender, Icon
	 */
	class Image:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Image, components::Accessibility>
	{
		public:
		/**
		 * @brief Configures Image widgets and registers them in the scene's
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<Image>
		{
			private:
			std::string _texturePath;	  ///< Path of the texture file to draw.
			std::string _fallbackPath;	  ///< Path of the texture drawn when
										  ///< the main one fails to load.
			float _width { 0.0f };	   ///< Displayed width, in dp (0: leave the
									   ///< Bound as created).
			float _height { 0.0f };	   ///< Displayed height, in dp (0: leave
									   ///< the Bound as created).

			public:
			/**
			 * @brief Construct a builder holding the defaults of reset().
			 * @param componentRegistry The registry the components of the
			 * built images are stored in.
			 * @param entityRegistry The registry that owns the images built
			 * by `makeImage`.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build an image from the current configuration, without
			 * registering it or giving it a parent.
			 *
			 * The size is applied only when both the width and the height
			 * are positive.
			 * @return The new image.
			 */
			std::shared_ptr<Image> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: empty texture path, no fallback
			 * texture, and no size (width and height 0).
			 */
			void reset(void) override;

			/**
			 * @brief Choose the texture to draw.
			 * @param texturePath The path of the texture file, as the
			 * resource provider of the application resolves it. Empty by
			 * default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withTexturePath(const std::string &texturePath);

			/**
			 * @brief Set the texture drawn when the main one fails to load.
			 * @param fallbackPath The path of the fallback texture file.
			 * Empty by default, which means no fallback.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withFallbackPath(const std::string &fallbackPath);

			/**
			 * @brief Set the displayed size.
			 *
			 * Unlike Image::setSize(), this never throws: the built image
			 * keeps the Bound it was created with unless both values are
			 * positive.
			 * @param width The displayed width, in dp. 0 by default.
			 * @param height The displayed height, in dp. 0 by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withSize(float width, float height);

			/**
			 * @brief Build an image, register it under `parent` and reset
			 * the builder.
			 *
			 * The fallback path and size set with the `with...` methods
			 * apply to the built image.
			 * @param parent The entity the image is attached to, or nullptr
			 * for a root entity.
			 * @param texturePath The path of the texture file to draw.
			 * @return The registered image.
			 */
			std::shared_ptr<Image> makeImage(std::shared_ptr<Entity> parent,
											 const std::string &texturePath);
		};

		public:
		/**
		 * @brief Build an image without registering it in an entity
		 * registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * `(registry, texturePath)`. An image has no children to build.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new image. The caller owns it.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Image> create(Arguments &&...arguments)
		{
			std::shared_ptr<Image> entity(
				new Image(std::forward<Arguments>(arguments)...));
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~Image(void);

		/**
		 * @brief Change the texture to draw.
		 *
		 * systems::ImageRender loads it at the next Render phase.
		 * @param texturePath The path of the new texture file, written to
		 * components::Image.
		 * @return Reference to this image for chaining.
		 */
		Image &setTexturePath(const std::string &texturePath);

		/**
		 * @brief Get the path of the texture the image was asked to draw.
		 * @return The texture path stored in components::Image. It does not
		 * change when the fallback is drawn instead.
		 */
		const std::string &getTexturePath(void) const;

		/**
		 * @brief Set the texture drawn when the main one fails to load.
		 * @param fallbackPath The path of the fallback texture file, or an
		 * empty string for no fallback.
		 * @return Reference to this image for chaining.
		 */
		Image &setFallbackPath(const std::string &fallbackPath);

		/**
		 * @brief Set the displayed size, written to components::Bound.
		 * @param width The width, in dp. Must be positive.
		 * @param height The height, in dp. Must be positive.
		 * @return Reference to this image for chaining.
		 * @throws std::invalid_argument If a dimension is zero or negative.
		 */
		Image &setSize(float width, float height);

		/**
		 * @brief Set the displayed size from a width and an aspect ratio.
		 *
		 * The height is `width / aspectRatio`: a width of 160 with a ratio
		 * of 16.0f / 9.0f gives a height of 90. The ratio is not stored;
		 * it is applied once.
		 * @param width The width, in dp. Must be positive.
		 * @param aspectRatio The width divided by the height. Must be
		 * positive.
		 * @return Reference to this image for chaining.
		 * @throws std::invalid_argument If an argument is zero or negative.
		 */
		Image &setAspectRatio(float width, float aspectRatio);

		/**
		 * @brief Tell whether the texture could be loaded.
		 * @return `Pending` before systems::ImageRender first ran on the
		 * image, `Loaded` when the texture or its fallback is drawn,
		 * `Failed` when neither could be loaded.
		 */
		components::Image::LoadState getLoadState(void) const;

		/**
		 * @brief Describe the picture for assistive technologies.
		 * @param label The text announced for the image, written to
		 * components::Accessibility. The role is always Image.
		 * @return Reference to this image for chaining.
		 */
		Image &setAccessibilityLabel(const std::string &label);

		private:
		/**
		 * @brief Construct an image and give it the Image accessibility
		 * role.
		 * @param registry The registry the components are stored in.
		 * @param texturePath The path of the texture file to draw.
		 */
		Image(ecs::ComponentRegistry &registry, const std::string &texturePath);
	};

}	 // namespace guillaume::entities
