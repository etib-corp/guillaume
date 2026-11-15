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
	 * @brief State of a tonal surface: how high it sits above the
	 * background, and whether it casts a shadow.
	 *
	 * The entities::Surface widget writes it. systems::SurfaceTone reads it
	 * every frame in the Measure phase and overwrites two other components
	 * of the entity: Color (the theme's surface color tinted for the
	 * level) and Elevation (the shadow, level 0 when the shadow is off).
	 * So on an entity with this component, set the level here, not on
	 * Color or Elevation.
	 *
	 * @see systems::SurfaceTone, components::Elevation, entities::Surface
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
		 * @brief Create the state of a level 1 surface with a shadow.
		 */
		SurfaceTone(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~SurfaceTone(void) = default;

		/**
		 * @brief Get how high the surface sits above the background.
		 * @return The elevation level, Level1 by default.
		 */
		theme::ElevationLevel getElevation(void) const noexcept;

		/**
		 * @brief Set how high the surface sits above the background.
		 *
		 * A higher level gives a stronger tint and, with the shadow on, a
		 * larger shadow. Marks the component changed only when the level
		 * differs.
		 * @param elevation The new elevation level.
		 * @return This component, for chaining.
		 */
		SurfaceTone &setElevation(theme::ElevationLevel elevation);

		/**
		 * @brief Tell whether the surface casts a shadow.
		 * @return True when it does, which is the default.
		 */
		bool hasShadow(void) const noexcept;

		/**
		 * @brief Turn the shadow of the surface on or off.
		 *
		 * Without a shadow the surface keeps the tint of its level. Marks
		 * the component changed only when the flag differs.
		 * @param shadow True to cast a shadow.
		 * @return This component, for chaining.
		 */
		SurfaceTone &setShadow(bool shadow);
	};

}	 // namespace guillaume::components
