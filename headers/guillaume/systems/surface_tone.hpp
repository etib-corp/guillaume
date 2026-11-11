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
	 * @brief Resolves a tonal surface's color and shadow from its elevation.
	 *
	 * Reads components::SurfaceTone and the active theme, and writes the
	 * entity's Color and Elevation. Runs in the Measure phase so the render
	 * systems of the same frame see the result.
	 */
	class SurfaceTone:
		public ecs::SystemFiller<components::SurfaceTone, components::Color,
								 components::Elevation>
	{
		public:
		/**
		 * @brief Default constructor.
		 */
		SurfaceTone(void);

		/**
		 * @brief Default destructor.
		 */
		~SurfaceTone(void) override = default;

		/**
		 * @brief Resolve one surface's color and shadow.
		 * @param componentRegistry The registry holding the components.
		 * @param entityIdentifier The surface to resolve.
		 */
		static void apply(ecs::ComponentRegistry &componentRegistry,
						  const ecs::Entity::Identifier &entityIdentifier);

		/**
		 * @brief Resolve the surface with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
