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

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief A translucent veil drawn over the whole bound of its entity,
	 * to dim what is behind a modal surface.
	 *
	 * systems::ScrimRender reads it in the Render phase and draws one quad
	 * of this color covering the entity's Transform and Bound. On a
	 * backdrop entity, systems::Backdrop overwrites the color every frame
	 * to fade the veil in and out.
	 *
	 * @see systems::ScrimRender, components::Backdrop
	 */
	class Scrim: public ecs::Component
	{
		private:
		utility::graphic::Color32Bit _color { 0, 0, 0,
											  128 };	///< Scrim color.

		public:
		/**
		 * @brief Create a half-transparent black scrim.
		 */
		Scrim(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~Scrim(void) = default;

		/**
		 * @brief Get the color of the veil.
		 * @return The color; its alpha is the strength of the dimming (0
		 * is invisible, 255 hides what is behind).
		 */
		utility::graphic::Color32Bit getColor(void) const;

		/**
		 * @brief Set the color of the veil.
		 *
		 * Marks the component changed only when the color differs.
		 * @param color The new color; its alpha is the strength of the
		 * dimming.
		 * @return This component, for chaining.
		 */
		Scrim &setColor(const utility::graphic::Color32Bit &color);
	};

}	 // namespace guillaume::components
