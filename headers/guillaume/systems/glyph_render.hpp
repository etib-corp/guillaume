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

#include <utility/ressource_provider.hpp>

#include <utility/graphic/text/text.hpp>

#include <utility/cache.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/glyph.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/bound.hpp"

#include "guillaume/engine.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Key structure for caching glyph rendering results.
	 *
	 * This structure is used as a key in the cache system to uniquely identify
	 * cached glyph rendering results based on the pose, glyph name, and color.
	 */
	struct GlyphRenderCacheKey {
		utility::graphic::PoseF pose;	 ///< The pose of the glyph, including
										 ///< position and orientation
		std::string glyphName;	  ///< The name of the glyph to be rendered
		float fontSize;	   ///< The font size used for rendering the glyph
		components::Glyph::Style
			glyphStyle;	   ///< The style of the glyph (e.g., filled, outlined)
		utility::graphic::Color32Bit
			color;	  ///< The color of the glyph, which may affect its
					  ///< rendering

		/**
		 * @brief Equality operator for GlyphRenderCacheKey.
		 * @param other The other GlyphRenderCacheKey to compare with.
		 * @return True if the keys are equal, false otherwise.
		 */
		bool operator==(const GlyphRenderCacheKey &other) const
		{
			if (pose != other.pose) {
				return false;
			}
			if (glyphName != other.glyphName) {
				return false;
			}
			if (fontSize != other.fontSize) {
				return false;
			}
			if (glyphStyle != other.glyphStyle) {
				return false;
			}
			if (color != other.color) {
				return false;
			}
			return true;
		}

		/**
		 * @brief Inequality operator for GlyphRenderCacheKey.
		 * @param other The other GlyphRenderCacheKey to compare with.
		 * @return True if the keys are not equal, false otherwise.
		 */
		bool operator!=(const GlyphRenderCacheKey &other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Less-than operator for GlyphRenderCacheKey.
		 * @param other The other GlyphRenderCacheKey to compare with.
		 * @return True if this key is less than the other, false otherwise.
		 */
		bool operator<(const GlyphRenderCacheKey &other) const
		{
			if (pose != other.pose) {
				return pose < other.pose;
			}
			if (glyphName != other.glyphName) {
				return glyphName < other.glyphName;
			}
			if (fontSize != other.fontSize) {
				return fontSize < other.fontSize;
			}
			if (glyphStyle != other.glyphStyle) {
				return glyphStyle < other.glyphStyle;
			}
			if (color != other.color) {
				return color < other.color;
			}
			return false;
		}
	};

	/**
	 * @brief Cache entry structure for glyph rendering results.
	 */
	struct GlyphRenderCacheEntry {
		bool used;	  ///< Flag indicating whether the cache entry has been used
					  ///< in the current frame
		size_t value;	 ///< The cached value associated with the glyph
						 ///< rendering result
	};

	/**
	 * @brief System handling glyph rendering from ECS components.
	 * @see components::Transform
	 * @see components::Glyph
	 * @see components::Color
	 * @see components::Bound
	 */
	class GlyphRender:
		public utility::Cache<GlyphRenderCacheKey, GlyphRenderCacheEntry>,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Glyph, components::Color>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   //< Shared resource provider for loading
								   // fonts and glyphs
		std::unique_ptr<Engine> &_engine;	 ///< Engine instance
		std::string _defaultFontPath;	 ///< Default font for glyph rendering
		std::string _glyphCodePath;		 ///< Path to glyph code mapping file
		std::shared_ptr<utility::graphic::CodePoints> _codePoints;

		public:
		/**
		 * @brief Construct a glyph rendering system.
		 * @param ressourceProvider Shared resource provider for loading fonts
		 * and glyphs.
		 * @param engine The engine used to draw glyph.
		 */
		GlyphRender(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			std::unique_ptr<Engine> &engine);

		/**
		 * @brief Default destructor.
		 */
		~GlyphRender(void);

		/**
		 * @brief Prepare the GlyphRender system for rendering.
		 *
		 * This function is called before call update on all entities and can be
		 * used to set up any necessary state or resources.
		 */
		void prepare(void) override;

		/**
		 * @brief Clean up the GlyphRender system after rendering.
		 *
		 * This function is called after call update on all entities and can be
		 * used to release any resources or reset state.
		 */
		void cleanup(void) override;

		/**
		 * @brief Update the GlyphRender system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
