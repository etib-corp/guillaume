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
#include <string>

#include <utility/ressource_provider.hpp>

#include <utility/cache.hpp>

#include <utility/graphic/text/code_points.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/glyph.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"

#include "guillaume/engine.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Structure representing a cache key for measuring glyphs.
	 */
	struct MeasureGlyphCacheKey {
		std::string glyphName;	  ///< The name of the glyph to be measured
		float fontSize;	   ///< The font size used for measuring the glyph
		components::Glyph::Style
			glyphStyle;	   ///< The style of the glyph (e.g., filled, outlined)
		utility::graphic::Color32Bit
			color;	  ///< The color of the glyph, which may affect its
					  ///< rendering and measurement

		/**
		 * @brief Equality operator for MeasureGlyphCacheKey.
		 * @param other The other MeasureGlyphCacheKey to compare with.
		 * @return True if the keys are equal, false otherwise.
		 */
		bool operator==(const MeasureGlyphCacheKey &other) const
		{
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
		 * @brief Inequality operator for MeasureGlyphCacheKey.
		 * @param other The other MeasureGlyphCacheKey to compare with.
		 * @return True if the keys are not equal, false otherwise.
		 */
		bool operator!=(const MeasureGlyphCacheKey &other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Less-than operator for MeasureGlyphCacheKey.
		 * @param other The other MeasureGlyphCacheKey to compare with.
		 * @return True if this key is less than the other, false otherwise.
		 */
		bool operator<(const MeasureGlyphCacheKey &other) const
		{
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
	 * @brief System measuring glyphs and synchronizing them to bound sizes.
	 * @see components::Glyph
	 * @see components::Bound
	 * @see components::Transform
	 */
	class MeasureGlyph:
		public utility::Cache<MeasureGlyphCacheKey, utility::graphic::SizeF>,
		public ecs::SystemFiller<components::Glyph, components::Bound,
								 components::Transform, components::Color>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Shared resource provider for loading
								   ///< fonts and glyphs
		std::unique_ptr<Engine>
			&_engine;	 ///< Engine instance for text measurement
		std::string
			_defaultFontPath;	 ///< Default font used for text measurement
		std::string _glyphCodePath;	   ///< Path to glyph code mapping file
		std::shared_ptr<utility::graphic::CodePoints> _codePoints;

		public:
		/**
		 * @brief Construct a glyph measuring system.
		 * @param ressourceProvider Shared resource provider for loading fonts
		 * and glyphs.
		 * @param engine The engine used to measure text.
		 */
		MeasureGlyph(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			std::unique_ptr<Engine> &engine);

		/**
		 * @brief Default destructor.
		 */
		~MeasureGlyph(void);

		/**
		 * @brief Prepare the MeasureGlyph system before measurement.
		 *
		 * This function is called before call update on all entities and can be
		 * used to set up any necessary state or resources.
		 */
		void prepare(void) override;

		/**
		 * @brief Clean up the MeasureGlyph system after measurement.
		 *
		 * This function is called after call update on all entities and can be
		 * used to release any resources or reset state.
		 */
		void cleanup(void) override;

		/**
		 * @brief Update the MeasureGlyph system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
