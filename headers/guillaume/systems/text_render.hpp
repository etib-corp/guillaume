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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/renderer.hpp"

namespace guillaume::systems
{

	/**
	 * @brief System handling text rendering from ECS components.
	 * @see components::Text
	 * @see components::Transform
	 */
	class TextRender:
		public ecs::SystemFiller<components::Transform, components::Text>
	{
		private:
		Renderer &_renderer;			 ///< Renderer instance
		std::string _defaultFontPath;	 ///< Default font for text rendering

		public:
		/**
		 * @brief Construct a text rendering system.
		 * @param renderer The renderer used to draw text.
		 */
		TextRender(Renderer &renderer);

		/**
		 * @brief Default destructor.
		 */
		~TextRender(void);

		/**
		 * @brief Update the TextRender system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems