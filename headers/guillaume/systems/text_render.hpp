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

#include <map>
#include <optional>

#include <utility/cache.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"

#include "guillaume/engine.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Key structure for caching text rendering results.
	 */
	struct TextRenderCacheKey {
		utility::graphic::PoseF pose;	 ///< The pose of the text, including
										 ///< position and orientation
		std::string content;			 ///< The text content to be rendered
		float fontSize;	   ///< The font size used for rendering the text
		utility::graphic::Color32Bit
			color;	  ///< The color of the text, which may affect its rendering

		/**
		 * @brief Equality operator for TextRenderCacheKey.
		 * @param other The other TextRenderCacheKey to compare with.
		 * @return True if the keys are equal, false otherwise.
		 */
		bool operator==(const TextRenderCacheKey &other) const
		{
			if (pose != other.pose) {
				return false;
			}
			if (content != other.content) {
				return false;
			}
			if (fontSize != other.fontSize) {
				return false;
			}
			if (color != other.color) {
				return false;
			}
			return true;
		}

		/**
		 * @brief Inequality operator for TextRenderCacheKey.
		 * @param other The other TextRenderCacheKey to compare with.
		 * @return True if the keys are not equal, false otherwise.
		 */
		bool operator!=(const TextRenderCacheKey &other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Less-than operator for TextRenderCacheKey.
		 * @param other The other TextRenderCacheKey to compare with.
		 * @return True if this key is less than the other, false otherwise.
		 */
		bool operator<(const TextRenderCacheKey &other) const
		{
			if (pose != other.pose) {
				return pose < other.pose;
			}
			if (content != other.content) {
				return content < other.content;
			}
			if (fontSize != other.fontSize) {
				return fontSize < other.fontSize;
			}
			if (color != other.color) {
				return color < other.color;
			}
			return false;
		}
	};

	/**
	 * @brief Entry structure for caching text rendering results.
	 */
	struct TextRenderCacheEntry {
		bool used;	  ///< Flag indicating whether the cache entry has been used
					  ///< in the current frame
		size_t value;	 ///< The cached value associated with the text
						 ///< rendering result
	};

	/**
	 * @brief System handling text rendering from ECS components.
	 * @see components::Text
	 * @see components::Transform
	 */
	class TextRender:
		public utility::Cache<TextRenderCacheKey, TextRenderCacheEntry>,
		public ecs::SystemFiller<components::Transform, components::Text,
								 components::Color>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Shared resource provider for loading
								   ///< fonts and glyphs
		std::unique_ptr<Engine> &_engine;	 ///< Engine instance
		std::string _defaultFontPath;	 ///< Default font for text rendering

		public:
		/**
		 * @brief Construct a text rendering system.
		 * @param ressourceProvider Shared resource provider for loading fonts
		 * and glyphs.
		 * @param engine The engine used to draw text.
		 */
		TextRender(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			std::unique_ptr<Engine> &engine);

		/**
		 * @brief Default destructor.
		 */
		~TextRender(void);

		/**
		 * @brief Prepare the TextRender system before rendering.
		 *
		 * This function is called before call update on all entities and can be
		 * used to set up any necessary state or resources.
		 */
		void prepare(void) override;

		/**
		 * @brief Clean up the TextRender system after rendering.
		 *
		 * This function is called after call update on all entities and can be
		 * used to release any resources or reset state.
		 */
		void cleanup(void) override;

		/**
		 * @brief Update the TextRender system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems