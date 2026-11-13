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
	 * @brief Main color of an entity: the fill of a shape, or the ink of a
	 * text or an icon.
	 *
	 * Read by the render systems (systems::RectangleRender,
	 * systems::EllipseRender, systems::RingRender, systems::ArcRender,
	 * systems::LineRender, systems::TextRender, systems::GlyphRender) and,
	 * as part of their cache key, by systems::MeasureText and
	 * systems::MeasureGlyph. Written by the application for a plain entity
	 * and by the widget systems for the entities of a widget.
	 *
	 * @code
	 * registry.getComponent<components::Color>(id).setColor(
	 * 	utility::graphic::Color32Bit(103, 80, 164, 255));
	 * @endcode
	 *
	 * @note A widget system restyles its entities every frame from the
	 * active theme and the widget state, so a color set by hand on a
	 * widget's entity does not last. Use the widget's own color setters.
	 * @note An alpha of 0 makes the fill invisible; the entity still takes
	 * part in layout and input.
	 *
	 * @see components::Borders for the stroke around a rectangle,
	 * components::Line::setColor for a stroke color of its own.
	 */
	class Color: public ecs::Component
	{
		private:
		utility::graphic::Color32Bit _color;	///< RGBA, 8 bits per channel

		public:
		/**
		 * @brief Construct with the default value of
		 * `utility::graphic::Color32Bit`.
		 */
		explicit Color(void);

		/**
		 * @brief Default destructor for the Color component.
		 */
		~Color(void) = default;

		/**
		 * @brief Get the color.
		 * @return The RGBA color, 8 bits per channel (alpha 255 is
		 * opaque).
		 */
		const utility::graphic::Color32Bit &getColor() const;

		/**
		 * @brief Set the color.
		 *
		 * Marks the component changed only when @p color differs from the
		 * stored one.
		 *
		 * @param color The RGBA color, 8 bits per channel.
		 * @return This component, for chaining.
		 */
		Color &setColor(const utility::graphic::Color32Bit &color);
	};

}	 // namespace guillaume::components