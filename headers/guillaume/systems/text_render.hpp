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

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"

#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Draws the string of every entity that has a components::Text.
	 *
	 * Runs in the Render phase on entities with Transform, Text and Color.
	 * It reads the three and writes no component: its output is one engine
	 * object per entity (a TextRenderable), rebuilt and uploaded every
	 * frame.
	 * - components::Transform gives the pose the text is drawn at.
	 * - components::Text gives the string, the font size and the font
	 *   weight. The font file is chosen from the weight by
	 *   theme::fontPath.
	 * - components::Color gives the text color.
	 *
	 * The size of the text is not computed here: systems::MeasureText writes
	 * it into components::Bound during the Measure phase, with the same
	 * font, and the layout uses it. systems::Typography fills the font size
	 * and weight from a type-scale role before that.
	 *
	 * @see components::Text, systems::MeasureText, systems::Typography
	 * @see RenderHandleMap
	 */
	class TextRender:
		public RenderHandleMap,
		public ecs::SystemFiller<components::Transform, components::Text,
								 components::Color>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;		 ///< Provider the renderables load their
									 ///< font from
		utility::Engine &_engine;	 ///< Engine the texts are uploaded to
		std::string
			_defaultFontPath;	 ///< Path of the regular text font. The font
								 ///< actually drawn comes from theme::fontPath

		public:
		/**
		 * @brief Create the system in the Render phase.
		 * @param ressourceProvider Provider used to load the fonts.
		 * @param engine The engine the texts are uploaded to. It must
		 * outlive the system.
		 */
		TextRender(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			utility::Engine &engine);

		/**
		 * @brief Forget the engine objects without removing them from the
		 * engine.
		 */
		~TextRender(void);

		/**
		 * @brief Mark every text as not drawn yet. Called once per frame
		 * before the entities are visited.
		 */
		void prepare(void) override;

		/**
		 * @brief Remove the engine objects of the entities that were not
		 * drawn this frame. Called once per frame after the entities.
		 */
		void cleanup(void) override;

		/**
		 * @brief Build the renderable of one text from its current
		 * components and upload it, creating the engine object on the first
		 * frame.
		 * @param entityIdentifier The entity to draw. It is skipped, with a
		 * log message, when one of the three components is missing.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
