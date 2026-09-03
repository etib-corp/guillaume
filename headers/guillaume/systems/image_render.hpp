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

#include <utility/ressource_provider.hpp>

#include <utility/cache.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/image.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"

#include "guillaume/engine.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Key structure for caching image rendering results.
	 *
	 * This structure is used as a key in the cache system to uniquely identify
	 * cached image rendering results based on the pose, size, and texture path.
	 */
	struct ImageRenderCacheKey {
		utility::graphic::PoseF pose;	 ///< The pose of the image
		utility::graphic::SizeF size;	 ///< The size of the image
		std::string texturePath;	 ///< The path of the image texture

		/**
		 * @brief Equality operator for ImageRenderCacheKey.
		 * @param other The other ImageRenderCacheKey to compare with.
		 * @return True if the keys are equal, false otherwise.
		 */
		bool operator==(const ImageRenderCacheKey &other) const
		{
			if (pose != other.pose) {
				return false;
			}
			if (size != other.size) {
				return false;
			}
			if (texturePath != other.texturePath) {
				return false;
			}
			return true;
		}

		/**
		 * @brief Inequality operator for ImageRenderCacheKey.
		 * @param other The other ImageRenderCacheKey to compare with.
		 * @return True if the keys are not equal, false otherwise.
		 */
		bool operator!=(const ImageRenderCacheKey &other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Less-than operator for ImageRenderCacheKey.
		 * @param other The other ImageRenderCacheKey to compare with.
		 * @return True if this key is less than the other, false otherwise.
		 */
		bool operator<(const ImageRenderCacheKey &other) const
		{
			if (pose != other.pose) {
				return pose < other.pose;
			}
			if (size != other.size) {
				return size < other.size;
			}
			if (texturePath != other.texturePath) {
				return texturePath < other.texturePath;
			}
			return false;
		}
	};

	/**
	 * @brief Cache entry structure for image rendering results.
	 */
	struct ImageRenderCacheEntry {
		bool used;	  ///< Flag indicating whether the cache entry has been used
					  ///< in the current frame
		size_t value;	 ///< The cached value associated with the image
						 ///< rendering result
	};

	/**
	 * @brief System handling image rendering from ECS components.
	 * @see components::Transform
	 * @see components::Bound
	 * @see components::Image
	 */
	class ImageRender:
		public utility::Cache<ImageRenderCacheKey, ImageRenderCacheEntry>,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Image>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   //< Shared resource provider for loading
								   // image materials
		std::unique_ptr<Engine> &_engine;	 ///< Engine instance

		public:
		/**
		 * @brief Construct an image rendering system.
		 * @param ressourceProvider Shared resource provider for loading image
		 * materials.
		 * @param engine The engine used to draw images.
		 */
		ImageRender(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			std::unique_ptr<Engine> &engine);

		/**
		 * @brief Default destructor.
		 */
		~ImageRender(void);

		/**
		 * @brief Prepare the ImageRender system for rendering.
		 *
		 * This function is called before call update on all entities and can be
		 * used to set up any necessary state or resources.
		 */
		void prepare(void) override;

		/**
		 * @brief Clean up the ImageRender system after rendering.
		 *
		 * This function is called after call update on all entities and can be
		 * used to release any resources or reset state.
		 */
		void cleanup(void) override;

		/**
		 * @brief Update the ImageRender system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
