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

#include <string>

#include <utility/ressource_provider.hpp>

#include <utility/graphic/text/code_points.hpp>

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/glyph.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/bound.hpp"

#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws the icon of every entity that has a components::Glyph.
	 *
	 * An icon is one character of the Material Symbols icon font, chosen by
	 * name. This system runs in the Render phase on entities with Transform,
	 * Bound, Glyph and Color. It writes no component: its output is one
	 * engine object per entity (a TextRenderable), rebuilt and uploaded
	 * every frame.
	 * - components::Transform gives the pose the icon is drawn at.
	 * - components::Glyph gives the icon name and the font size.
	 * - components::Color gives the icon color.
	 * - components::Bound is required but not read here: it is filled by
	 *   systems::MeasureGlyph for the layout.
	 *
	 * Things to know:
	 * - A name that is not in the code point table, or a missing table, is
	 *   drawn as the character `?`.
	 * - When the icon font cannot be loaded nothing is drawn.
	 * - The style of the Glyph is not used: every icon is drawn with the
	 *   outlined font file.
	 *
	 * @see components::Glyph, systems::MeasureGlyph
	 * @see RenderHandleMap
	 */
	class GlyphRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Bound,
								 components::Glyph, components::Color>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;		 ///< Provider the icon font and the code
									 ///< point table are loaded from
		utility::Engine &_engine;	 ///< Engine the icons are uploaded to
		std::string _defaultFontPath;	 ///< Path of the icon font file
		std::string _glyphCodePath;	   ///< Path of the file mapping icon names
									   ///< to code points
		std::shared_ptr<utility::graphic::CodePoints>
			_codePoints;	///< Icon name to code point table, or nullptr
							///< when the file could not be loaded
		bool _hasFont { false };	///< Whether the icon font could be loaded

		public:
		/**
		 * @brief Create the system in the Render phase and load the icon
		 * font and its code point table.
		 *
		 * A missing font or table is logged and does not throw.
		 * @param ressourceProvider Provider used to load the font and the
		 * table. Must not be null.
		 * @param engine The engine the icons are uploaded to. It must
		 * outlive the system.
		 */
		GlyphRender(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			utility::Engine &engine);

		/**
		 * @brief Forget the engine objects without removing them from the
		 * engine.
		 */
		~GlyphRender(void);

		/**
		 * @brief Mark every icon as not drawn yet. Called once per frame
		 * before the entities are visited.
		 */
		void prepare(void) override;

		/**
		 * @brief Remove the engine objects of the entities that were not
		 * drawn this frame. Called once per frame after the entities.
		 */
		void cleanup(void) override;

		/**
		 * @brief Build the renderable of one icon from its current
		 * components and upload it, creating the engine object on the first
		 * frame.
		 * @param entityIdentifier The entity to draw. It is skipped when the
		 * icon font is missing, and skipped with a log message when one of
		 * the four components is missing.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
