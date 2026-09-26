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
	 * @brief Component drawing a translucent scrim over the entity bound.
	 *
	 * Used by modal dialogs, sheets, menus and snackbars to dim the content
	 * behind them. The scrim is rendered as a full-bound quad by
	 * `systems::ScrimRender`.
	 *
	 * @see systems::ScrimRender
	 */
	class Scrim: public ecs::Component
	{
		private:
		utility::graphic::Color32Bit _color { 0, 0, 0,
											  128 };	///< Scrim color.

		public:
		/**
		 * @brief Default constructor for the Scrim component.
		 */
		Scrim(void) = default;

		/**
		 * @brief Default destructor for the Scrim component.
		 */
		~Scrim(void) = default;

		/**
		 * @brief Get the scrim color.
		 * @return The scrim color.
		 */
		utility::graphic::Color32Bit getColor(void) const;

		/**
		 * @brief Set the scrim color.
		 * @param color The new scrim color.
		 * @return Reference to this Scrim for chaining.
		 */
		Scrim &setColor(const utility::graphic::Color32Bit &color);
	};

}	 // namespace guillaume::components
