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

#include "guillaume/ecs/component.hpp"
#include "guillaume/theme/tokens.hpp"

namespace guillaume::components
{

	/**
	 * @brief Configuration of a tonal surface: its elevation and whether it
	 * casts a shadow.
	 *
	 * This component is the whole state of entities::Surface.
	 * systems::SurfaceTone turns it into the entity's Color (the tonal
	 * surface color of the level) and Elevation (the shadow).
	 */
	class SurfaceTone: public ecs::Component
	{
		private:
		theme::ElevationLevel _elevation {
			theme::ElevationLevel::Level1
		};	  ///< Elevation level

		bool _shadow { true };	  ///< Whether the surface casts a shadow

		public:
		/**
		 * @brief Default constructor: level 1, with a shadow.
		 */
		SurfaceTone(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~SurfaceTone(void) = default;

		/** @brief Get the elevation level. @return The level. */
		theme::ElevationLevel getElevation(void) const noexcept;

		/** @brief Set the elevation level. @param elevation It. @return It. */
		SurfaceTone &setElevation(theme::ElevationLevel elevation);

		/** @brief Whether a shadow is cast. @return True when it is. */
		bool hasShadow(void) const noexcept;

		/** @brief Enable or disable the shadow. @param shadow It. @return It.
		 */
		SurfaceTone &setShadow(bool shadow);
	};

}	 // namespace guillaume::components
