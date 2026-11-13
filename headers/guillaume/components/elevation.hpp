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
#include "guillaume/theme/tokens.hpp"

namespace guillaume::components
{

	/**
	 * @brief Soft shadow drawn around an elevated surface.
	 *
	 * systems::ElevationRender (Render phase) reads it with the entity's
	 * Transform and Bound. It draws getLayers() rounded shapes centered on
	 * the bound: layer `i` (from 1 to the layer count) is larger than the
	 * bound by `spread * level * i / layers` on every side and its alpha is
	 * the shadow alpha times `1 - i / layers`, which approximates a blurred
	 * shadow.
	 *
	 * The usual way to set it is setElevationLevel(), which resolves the
	 * spread and the opacity from the elevation scale of the active theme.
	 * Widget systems do so for their surfaces (systems::SurfaceTone,
	 * systems::Card, the button systems, ...); setLevel(), setSpread() and
	 * setColor() then adjust one value, for example while animating
	 * between two levels.
	 *
	 * @code
	 * registry.addComponent<components::Elevation>(id).setElevationLevel(
	 * 	theme::ElevationLevel::Level2);
	 * @endcode
	 *
	 * @note setElevationLevel() overwrites the level factor, the spread
	 * and the color: call the fine-grained setters after it, never before.
	 * @note A freshly constructed component is at Level1, so adding it to
	 * an entity is enough to draw a shadow.
	 *
	 * @see systems::ElevationRender, theme::ElevationLevel
	 */
	class Elevation: public ecs::Component
	{
		private:
		float _level { 1.0f };	   ///< Spread factor; 0 paints no shadow.
		int _layers { 4 };		   ///< Number of fading shadow layers.
		float _spread { 0.0f };	   ///< Shadow reach, in dp, at level 1.
		utility::graphic::Color32Bit _color {
			0, 0, 0, 0
		};	  ///< Shadow color; its alpha is the opacity.

		theme::ElevationLevel _elevationLevel {
			theme::ElevationLevel::Level1
		};	  ///< Token last applied by setElevationLevel().

		public:
		/**
		 * @brief Construct an elevation resolved from the Level1 token of
		 * the active theme. The component starts unchanged.
		 */
		Elevation(void);

		/**
		 * @brief Default destructor for the Elevation component.
		 */
		~Elevation(void) = default;

		/**
		 * @brief Get the factor applied to the spread.
		 * @return 0 when no shadow is painted, 1 after setElevationLevel()
		 * with any level above Level0, or the value given to setLevel().
		 */
		float getLevel(void) const;

		/**
		 * @brief Set the factor applied to the spread.
		 *
		 * The shadow reaches `spread * level` dp beyond the bound. This is
		 * a plain multiplier, not a theme::ElevationLevel; to pick a
		 * Material level use setElevationLevel(). Marks the component
		 * changed only when the value differs.
		 *
		 * @param level The factor; 0 or less paints no shadow.
		 * @return This component, for chaining.
		 */
		Elevation &setLevel(float level);

		/**
		 * @brief Get the number of shapes the shadow is made of.
		 * @return The layer count (4 by default).
		 */
		int getLayers(void) const;

		/**
		 * @brief Set the number of shapes the shadow is made of.
		 *
		 * More layers give a smoother fade for more triangles. Marks the
		 * component changed only when the value differs.
		 *
		 * @param layers The layer count; systems::ElevationRender draws at
		 * least 1 layer whatever the value.
		 * @return This component, for chaining.
		 */
		Elevation &setLayers(int layers);

		/**
		 * @brief Get how far the shadow reaches beyond the bound.
		 * @return The spread, in dp, for a level factor of 1.
		 */
		float getSpread(void) const;

		/**
		 * @brief Set how far the shadow reaches beyond the bound.
		 *
		 * Marks the component changed only when the value differs.
		 * setElevationLevel() replaces it with the token's spread.
		 *
		 * @param spread The spread, in dp, for a level factor of 1.
		 * @return This component, for chaining.
		 */
		Elevation &setSpread(float spread);

		/**
		 * @brief Get the shadow color.
		 * @return A copy of the color; its alpha sets how opaque the
		 * shadow is.
		 */
		utility::graphic::Color32Bit getColor(void) const;

		/**
		 * @brief Set the shadow color.
		 *
		 * Marks the component changed only when the color differs.
		 * setElevationLevel() replaces it with black at the token's
		 * opacity.
		 *
		 * @param color The color; an alpha of 0 paints no shadow.
		 * @return This component, for chaining.
		 */
		Elevation &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Resolve the shadow from a Material 3 elevation level.
		 *
		 * Reads the token of @p level in the active theme and writes three
		 * values: the level factor (0 for Level0, 1 otherwise), the spread
		 * and the color (black at the token's opacity). Level0 therefore
		 * paints no shadow. The component is marked changed only when one
		 * of the resolved values differs, so calling it every frame is
		 * cheap and still picks up a theme change.
		 *
		 * @param level The Material 3 elevation level.
		 * @return This component, for chaining.
		 */
		Elevation &setElevationLevel(theme::ElevationLevel level);

		/**
		 * @brief Get the level last applied with setElevationLevel().
		 * @return The level (Level1 on a new component). It does not
		 * reflect later calls to setLevel(), setSpread() or setColor().
		 */
		theme::ElevationLevel getElevationLevel(void) const;

		/**
		 * @brief Check whether a visible shadow must be painted.
		 * @return True when the level factor is above 0 and the alpha of
		 * the shadow color is not 0.
		 */
		bool hasShadow(void) const;
	};

}	 // namespace guillaume::components
