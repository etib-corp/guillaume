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

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/glyph.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Key of the size cache of MeasureGlyph: everything a measured
	 * icon size is looked up by.
	 *
	 * Two icons with equal keys share one measurement. The ordering
	 * operator exists so the key can be used in the ordered map of
	 * utility::Cache.
	 * @see MeasureGlyph
	 */
	struct MeasureGlyphCacheKey {
		std::string glyphName;	  ///< Name of the icon, such as "home"
		float fontSize;			  ///< Font size the icon was measured at
		components::Glyph::Style
			glyphStyle;	   ///< Style of the icon (outlined, rounded, sharp)
		utility::graphic::Color32Bit
			color;	  ///< Color of the icon. It does not change the size,
					  ///< but two colors give two cache entries

		/**
		 * @brief Compare two keys field by field.
		 * @param other The key to compare with.
		 * @return True when name, font size, style and color are all equal.
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
		 * @brief Negation of operator==.
		 * @param other The key to compare with.
		 * @return True when at least one field differs.
		 */
		bool operator!=(const MeasureGlyphCacheKey &other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Order two keys by name, then font size, then style, then
		 * color.
		 * @param other The key to compare with.
		 * @return True when this key sorts before @p other.
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
	 * @brief Computes the size of every icon and writes it into its Bound,
	 * so the layout knows how much room the icon takes.
	 *
	 * An icon is a components::Glyph: a name looked up in the Material
	 * Symbols icon font. This system runs in the Measure phase on entities
	 * with Glyph, Bound, Transform and Color.
	 * - Reads: the name, font size and style of components::Glyph.
	 * - Writes: the width and height of components::Bound.
	 *
	 * The measurement is asked to the engine
	 * (utility::Engine::measureText) once per distinct
	 * MeasureGlyphCacheKey and kept for the life of the system; later
	 * frames only copy the cached size.
	 *
	 * Things to know:
	 * - The Bound of an icon entity is overwritten every frame.
	 * - A name that is not in the code point table, or a missing table, is
	 *   measured as the character `?`, which is also what
	 *   systems::GlyphRender draws.
	 * - When the icon font cannot be loaded the system does nothing and the
	 *   Bound keeps its value.
	 * - Every style is measured with the same font file (the outlined
	 *   one).
	 *
	 * @see components::Glyph, systems::GlyphRender
	 * @see MeasureGlyphCacheKey
	 */
	class MeasureGlyph:
		public utility::Cache<MeasureGlyphCacheKey, utility::graphic::SizeF>,
		public ecs::SystemFiller<components::Glyph, components::Bound,
								 components::Transform, components::Color>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;		 ///< Provider the icon font and the code
									 ///< point table are loaded from
		utility::Engine &_engine;	 ///< Engine asked to measure the icons
		std::string _defaultFontPath;	 ///< Path of the icon font file
		std::string _glyphCodePath;	   ///< Path of the file mapping icon names
									   ///< to code points
		std::shared_ptr<utility::graphic::CodePoints>
			_codePoints;	///< Icon name to code point table, or nullptr
							///< when the file could not be loaded
		bool _hasFont { false };	///< Whether the icon font could be loaded

		public:
		/**
		 * @brief Create the system in the Measure phase and load the icon
		 * font and its code point table.
		 *
		 * A missing font or table is logged and does not throw.
		 * @param ressourceProvider Provider used to load the font and the
		 * table. Must not be null.
		 * @param engine The engine asked to measure the icons. It must
		 * outlive the system.
		 */
		MeasureGlyph(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			utility::Engine &engine);

		/**
		 * @brief Destroy the system and its cache.
		 */
		~MeasureGlyph(void);

		/**
		 * @brief Does nothing: the system needs no per-frame setup.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing: the cache is kept from one frame to the next.
		 */
		void cleanup(void) override;

		/**
		 * @brief Write the size of one icon into its components::Bound,
		 * measuring it with the engine when it is not in the cache yet.
		 * @param entityIdentifier The icon entity. It is skipped when the
		 * icon font is missing, and skipped with a log message when one of
		 * the four components is missing.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
