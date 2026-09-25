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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/glyph.hpp"
#include "guillaume/components/transform.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/bound.hpp"

#include "guillaume/engine.hpp"
#include "guillaume/systems/render_handle_map.hpp"

namespace guillaume::systems
{
	/**
	 * @brief System handling glyph rendering from ECS components.
	 * @see components::Transform
	 * @see components::Glyph
	 * @see components::Color
	 * @see components::Bound
	 */
	class GlyphRender:
		public RenderHandleMap,
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
