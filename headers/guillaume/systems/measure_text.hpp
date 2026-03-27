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

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/text.hpp"
#include "guillaume/components/transform.hpp"

#include "guillaume/renderer.hpp"

namespace guillaume::systems
{

	/**
	 * @brief System measuring text and synchronizing it to bound sizes.
	 * @see components::Text
	 * @see components::Bound
	 * @see components::Transform
	 */
	class MeasureText:
		public ecs::SystemFiller<components::Transform, components::Text,
								 components::Bound>
	{
		private:
		Renderer &_renderer;	///< Renderer instance
		std::string
			_defaultFontPath;	 ///< Default font used for text measurement

		public:
		/**
		 * @brief Construct a text measuring system.
		 * @param renderer The renderer used to measure text.
		 */
		MeasureText(Renderer &renderer);

		/**
		 * @brief Default destructor.
		 */
		~MeasureText(void);

		/**
		 * @brief Update the MeasureText system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
