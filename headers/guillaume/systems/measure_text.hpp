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

#include "guillaume/theme/typescale.hpp"
#include <string>

#include <utility/cache.hpp>

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Key of the size cache of MeasureText: everything a measured
	 * size is looked up by.
	 *
	 * Two texts with equal keys share one measurement. The ordering
	 * operator exists so the key can be used in the ordered map of
	 * utility::Cache.
	 * @see MeasureText
	 */
	struct MeasureTextCacheKey {
		std::string content;	///< The string that was measured
		float fontSize;			///< Font size the string was measured at
		utility::graphic::Color32Bit
			color;	  ///< Color of the text. It does not change the size,
					  ///< but two colors give two cache entries
		theme::FontWeight fontWeight {
			theme::FontWeight::Regular
		};	  ///< Font weight, which selects the font file measured with

		/**
		 * @brief Compare two keys field by field.
		 * @param other The key to compare with.
		 * @return True when content, font size, color and font weight are
		 * all equal.
		 */
		bool operator==(const MeasureTextCacheKey &other) const
		{
			if (content != other.content) {
				return false;
			}
			if (fontSize != other.fontSize) {
				return false;
			}
			if (color != other.color) {
				return false;
			}
			if (fontWeight != other.fontWeight) {
				return false;
			}
			return true;
		}

		/**
		 * @brief Negation of operator==.
		 * @param other The key to compare with.
		 * @return True when at least one field differs.
		 */
		bool operator!=(const MeasureTextCacheKey &other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Order two keys by content, then font size, then color,
		 * then font weight.
		 * @param other The key to compare with.
		 * @return True when this key sorts before @p other.
		 */
		bool operator<(const MeasureTextCacheKey &other) const
		{
			if (content != other.content) {
				return content < other.content;
			}
			if (fontSize != other.fontSize) {
				return fontSize < other.fontSize;
			}
			if (color != other.color) {
				return color < other.color;
			}
			return fontWeight < other.fontWeight;
		}
	};

	/**
	 * @brief Computes the size of every text and writes it into its Bound,
	 * so the layout knows how much room a label takes.
	 *
	 * Runs in the Measure phase on entities with Text, Bound, Transform and
	 * Color.
	 * - Reads: the string, font size, font weight and line height of
	 *   components::Text.
	 * - Writes: components::Bound. The width is the measured width; the
	 *   height is the measured height or the line height of the Text,
	 *   whichever is larger.
	 *
	 * The measurement is asked to the engine
	 * (utility::Engine::measureText) once per distinct
	 * MeasureTextCacheKey and kept for the life of the system; later frames
	 * only copy the cached size.
	 *
	 * Things to know:
	 * - The Bound of a text entity is overwritten every frame: a size set by
	 *   hand does not survive.
	 * - systems::Typography is registered before this system, in the same
	 *   phase, so a text styled by a type-scale role is measured with the
	 *   font size and weight of that role.
	 * - The cache is never emptied. A text whose content changes every
	 *   frame (a counter, a clock) adds one entry per distinct string.
	 *
	 * @see components::Text, systems::Typography, systems::TextRender
	 * @see MeasureTextCacheKey
	 */
	class MeasureText:
		public utility::Cache<MeasureTextCacheKey, utility::graphic::SizeF>,
		public ecs::SystemFiller<components::Text, components::Bound,
								 components::Transform, components::Color>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;			 ///< Provider the measured texts load
										 ///< their font from
		utility::Engine &_engine;		 ///< Engine asked to measure the texts
		std::string _defaultFontPath;	 ///< Path of the regular text font. The
										 ///< font actually measured comes from
										 ///< theme::fontPath

		public:
		/**
		 * @brief Create the system in the Measure phase, with an empty
		 * cache.
		 * @param ressourceProvider Provider used to load the fonts.
		 * @param engine The engine asked to measure the texts. It must
		 * outlive the system.
		 */
		MeasureText(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			utility::Engine &engine);

		/**
		 * @brief Destroy the system and its cache.
		 */
		~MeasureText(void);

		/**
		 * @brief Does nothing: the system needs no per-frame setup.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing: the cache is kept from one frame to the next.
		 */
		void cleanup(void) override;

		/**
		 * @brief Write the size of one text into its components::Bound,
		 * measuring it with the engine when it is not in the cache yet.
		 * @param entityIdentifier The text entity. It is skipped, with a log
		 * message, when one of the four components is missing.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
