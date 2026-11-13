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

#include "guillaume/components/color.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/surface_tone.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Gives a surface the color and the shadow that match its
	 * elevation level in the active theme.
	 *
	 * Runs in the Measure phase on entities with SurfaceTone, Color and
	 * Elevation.
	 * - Reads: components::SurfaceTone (the elevation level and whether the
	 *   surface casts a shadow) and the active theme.
	 * - Writes: components::Color, with the surface color the theme defines
	 *   for that level, and components::Elevation, with the shadow of that
	 *   level, or of level 0 (no shadow) when the surface casts none.
	 *
	 * Running in the Measure phase, ahead of the Render phase, means
	 * systems::RectangleRender and systems::ElevationRender draw the result
	 * in the same frame, and a change of theme reaches every surface on the
	 * next frame. Call apply() when the result is needed at once.
	 *
	 * The Color and the Elevation of an entity that has a SurfaceTone are
	 * overwritten every frame: change the SurfaceTone, not them.
	 *
	 * @see components::SurfaceTone, entities::Surface
	 * @see systems::ElevationRender
	 */
	class SurfaceTone:
		public ecs::SystemFiller<components::SurfaceTone, components::Color,
								 components::Elevation>
	{
		public:
		/**
		 * @brief Create the system in the Measure phase.
		 */
		SurfaceTone(void);

		/**
		 * @brief Destroy the system. It owns no resource.
		 */
		~SurfaceTone(void) override = default;

		/**
		 * @brief Write the color and the shadow of one surface from its
		 * components::SurfaceTone.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The surface entity. It must have
		 * SurfaceTone, Color and Elevation.
		 * @throws ecs::EntityComponentNotFoundException When one of the
		 * three components is missing.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Write the color and the shadow of one surface for this
		 * frame.
		 * @param entityIdentifier The surface entity. It is skipped, with a
		 * log message, when one of the three components is missing.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
