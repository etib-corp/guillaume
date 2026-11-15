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

namespace guillaume::components
{

	/**
	 * @brief Asks that drawing be cut to the entity's rectangle, and holds
	 * the rectangle that results.
	 *
	 * The application or a widget sets the margin and the enabled flag.
	 * systems::Clip (Layout phase) reads them with the entity's Transform
	 * and Bound, computes the rectangle (the bound shrunk by the margin on
	 * every side), stores it with setRect() and gives it to the engine as
	 * the scissor rectangle.
	 *
	 * @code
	 * registry.addComponent<components::Clip>(id).setMargin(8.0f);
	 * @endcode
	 *
	 * @note The rectangle is overwritten every frame while the clip is
	 * enabled; while it is disabled the system skips the entity and the
	 * rectangle keeps its last value.
	 * @note The rectangle is built from the X and Y of the position only:
	 * the orientation of the entity is ignored.
	 *
	 * @see systems::Clip
	 */
	class Clip: public ecs::Component
	{
		public:
		/**
		 * @brief The clip rectangle, in the same coordinates as the
		 * position of a components::Transform.
		 */
		struct Rect {
			float x { 0.0f };		  ///< Left edge, in dp.
			float y { 0.0f };		  ///< Top edge, in dp.
			float width { 0.0f };	  ///< Width, in dp.
			float height { 0.0f };	  ///< Height, in dp.

			/**
			 * @brief Compare the origin and the size of two rectangles.
			 * @param other The rectangle to compare with.
			 * @return True when the four values are equal.
			 */
			bool operator==(const Rect &other) const
			{
				return x == other.x && y == other.y && width == other.width
					&& height == other.height;
			}
		};

		private:
		float _margin { 0.0f };	   ///< Inset on every side, in dp.
		bool _enabled { true };	   ///< Whether clipping is active.
		Rect _rect {};			   ///< Written by systems::Clip.

		public:
		/**
		 * @brief Default constructor for the Clip component.
		 */
		Clip(void) = default;

		/**
		 * @brief Default destructor for the Clip component.
		 */
		~Clip(void) = default;

		/**
		 * @brief Get the inset between the bound and the clip rectangle.
		 * @return The margin, in dp, removed on every side (0 by default).
		 */
		float getMargin(void) const;

		/**
		 * @brief Set the inset between the bound and the clip rectangle.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param margin The margin, in dp, removed on every side. A
		 * negative value makes the rectangle larger than the bound.
		 * @return This component, for chaining.
		 */
		Clip &setMargin(float margin);

		/**
		 * @brief Check whether systems::Clip processes this entity.
		 * @return True when clipping is enabled (the default).
		 */
		bool isEnabled(void) const;

		/**
		 * @brief Turn clipping on or off without removing the component.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param enabled False to make systems::Clip skip this entity.
		 * @return This component, for chaining.
		 */
		Clip &setEnabled(bool enabled);

		/**
		 * @brief Get the rectangle computed by systems::Clip.
		 * @return The rectangle of the last frame the clip was enabled; a
		 * rectangle of zeros before the system ran once.
		 */
		const Rect &getRect(void) const;

		/**
		 * @brief Store the computed rectangle. Called by systems::Clip;
		 * a value set by hand is replaced on the next frame.
		 *
		 * Marks the component changed only when the rectangle differs.
		 *
		 * @param rect The clip rectangle, in dp.
		 * @return This component, for chaining.
		 */
		Clip &setRect(const Rect &rect);
	};

}	 // namespace guillaume::components
