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
	 * @brief Component describing a soft shadow below an elevated surface.
	 *
	 * The `systems::ElevationRender` system draws `layers` concentric rounded
	 * rectangles behind the entity `components::Bound`, expanding by
	 * `spread * level` and fading out toward the outside to approximate a
	 * blurred shadow.
	 *
	 * @see systems::ElevationRender
	 */
	class Elevation: public ecs::Component
	{
		private:
		float _level { 1.0f };	   ///< Elevation level driving the shadow size.
		int _layers { 4 };		   ///< Number of fading shadow layers.
		float _spread { 4.0f };	   ///< Expansion per elevation level.
		utility::graphic::Color32Bit _color {
			0, 0, 0, 64
		};	  ///< Shadow color (including opacity).

		public:
		/**
		 * @brief Default constructor for the Elevation component.
		 */
		Elevation(void) = default;

		/**
		 * @brief Default destructor for the Elevation component.
		 */
		~Elevation(void) = default;

		/**
		 * @brief Get the elevation level.
		 * @return The elevation level.
		 */
		float getLevel(void) const;

		/**
		 * @brief Set the elevation level.
		 * @param level The new elevation level.
		 * @return Reference to this Elevation for chaining.
		 */
		Elevation &setLevel(float level);

		/**
		 * @brief Get the number of shadow layers.
		 * @return The number of shadow layers.
		 */
		int getLayers(void) const;

		/**
		 * @brief Set the number of shadow layers.
		 * @param layers The new number of shadow layers.
		 * @return Reference to this Elevation for chaining.
		 */
		Elevation &setLayers(int layers);

		/**
		 * @brief Get the expansion per elevation level.
		 * @return The spread per elevation level.
		 */
		float getSpread(void) const;

		/**
		 * @brief Set the expansion per elevation level.
		 * @param spread The new spread per elevation level.
		 * @return Reference to this Elevation for chaining.
		 */
		Elevation &setSpread(float spread);

		/**
		 * @brief Get the shadow color.
		 * @return The shadow color.
		 */
		utility::graphic::Color32Bit getColor(void) const;

		/**
		 * @brief Set the shadow color.
		 * @param color The new shadow color.
		 * @return Reference to this Elevation for chaining.
		 */
		Elevation &setColor(const utility::graphic::Color32Bit &color);
	};

}	 // namespace guillaume::components
