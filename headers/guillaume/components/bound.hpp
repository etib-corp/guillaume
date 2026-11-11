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

#include <utility/graphic/size.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Size of an entity: the width and height, in dp, of the
	 * rectangle that starts at the position of its components::Transform.
	 *
	 * The bound has no position of its own. Shapes (rectangle, ellipse,
	 * ring, arc, line, image) are drawn to fill it, and the input systems
	 * hit-test against it.
	 *
	 * Who writes it depends on the entity:
	 * - a text, an icon or a model: systems::MeasureText,
	 *   systems::MeasureGlyph and systems::MeasureMesh write the measured
	 *   size every frame (Measure phase);
	 * - a container with a components::Layout: systems::Layout writes the
	 *   size that wraps its children, or the fixed size of the layout
	 *   (Layout phase);
	 * - a widget: its system writes the size the widget's configuration
	 *   asks for;
	 * - anything else: the application.
	 *
	 * @note In the first three cases a size set by hand is overwritten on
	 * the next frame. To force the size of a container use
	 * components::Layout::setFixedWidth and setFixedHeight.
	 * @note `getWidth()` and `getHeight()` come from
	 * `utility::graphic::SizeF`. Use the setters of this class, not the
	 * base class ones through a `SizeF` reference: only these mark the
	 * component changed.
	 *
	 * @see components::Transform, components::Layout, systems::MeasureText
	 */
	class Bound: public ecs::Component, public utility::graphic::SizeF
	{
		public:
		/**
		 * @brief Construct a bound with the default size of
		 * `utility::graphic::SizeF`.
		 */
		Bound(void);

		/**
		 * @brief Set the width.
		 *
		 * Hides `utility::graphic::SizeF::setWidth` to add change tracking:
		 * the component is marked changed only when @p width differs from
		 * the current width, so a measure system writing the same size
		 * every frame does not keep the entity dirty.
		 *
		 * @param width The new width, in dp.
		 * @return This component, for chaining.
		 */
		Bound &setWidth(float width) noexcept;

		/**
		 * @brief Set the height.
		 *
		 * Marks the component changed only when @p height differs from the
		 * current height.
		 *
		 * @param height The new height, in dp.
		 * @return This component, for chaining.
		 */
		Bound &setHeight(float height) noexcept;

		/**
		 * @brief Default destructor for the Bound component.
		 */
		~Bound(void) = default;
	};

}	 // namespace guillaume::components