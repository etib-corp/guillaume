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

#include <optional>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Look of a Material 3 common button: color style, corner shape,
	 * size and padding options.
	 *
	 * It sits on the same entity as components::Button, which holds the
	 * content and the interaction state. entities::Button writes it;
	 * systems::CommonButton reads both, each frame in the Layout phase and
	 * at once after every input change, and writes the button's Color,
	 * Borders, Elevation and Bound and the pose of its icon and label.
	 *
	 * @code
	 * registry.getComponent<components::CommonButton>(identifier)
	 * 	.setColorStyle(components::CommonButton::Color::Filled)
	 * 	.setSize(components::CommonButton::Size::Medium);
	 * @endcode
	 *
	 * Every setter compares before writing and marks the component changed
	 * only when the value differs.
	 *
	 * @see components::Button, systems::CommonButton,
	 * components::SegmentedButton
	 */
	class CommonButton: public ecs::Component
	{
		public:
		/**
		 * @brief Which theme colors fill the container, the content and the
		 * outline.
		 *
		 * Elevated, Filled and Tonal have a filled container; Outlined has a
		 * transparent container and an outline; Text has neither. Segmented
		 * is the style of one segment of a segmented button: outlined, and
		 * filled while selected without losing its outline.
		 */
		enum class Color { Elevated, Filled, Tonal, Outlined, Text, Segmented };

		/**
		 * @brief Corner shape at rest: fully rounded (Round) or the rounded
		 * rectangle of the size (Square).
		 */
		enum class Shape { Round, Square };

		/**
		 * @brief Size step. It sets the padding, the icon size, the gap
		 * between icon and label and the corner radius; the label always
		 * uses the label-large type role.
		 */
		enum class Size { ExtraSmall, Small, Medium, Large, ExtraLarge };

		/// Minimum width of the container, in dp.
		static constexpr float MinWidth = 64.0f;

		/// Minimum height of the container, in dp. ExtraSmall buttons are
		/// exempt.
		static constexpr float MinHeight = 40.0f;

		/**
		 * @brief Radius of each corner, for a button joined to its neighbours
		 * (a segment of a segmented button).
		 */
		struct Corners {
			float topLeft { 0.0f };		   ///< Top-left radius, in dp.
			float topRight { 0.0f };	   ///< Top-right radius, in dp.
			float bottomRight { 0.0f };	   ///< Bottom-right radius, in dp.
			float bottomLeft { 0.0f };	   ///< Bottom-left radius, in dp.

			/**
			 * @brief Compare two corner sets.
			 * @param other The other corner set.
			 * @return True when every radius is equal.
			 */
			bool operator==(const Corners &other) const = default;
		};

		private:
		Color _colorStyle { Color::Text };	  ///< Color style
		Shape _shape { Shape::Round };		  ///< Corner shape at rest
		Size _size { Size::Small };			  ///< Size step
		bool _compact {
			false
		};	  ///< Whether the horizontal padding is halved

		float _minWidth { 0.0f };	  ///< Requested minimum width, in dp
		bool _uppercase { false };	  ///< Whether the label is upper-cased
		std::optional<Corners> _corners {};	   ///< Per-corner radius override

		public:
		/**
		 * @brief Create the configuration of a small, round text button.
		 */
		CommonButton(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~CommonButton(void) = default;

		/**
		 * @brief Get the color style.
		 * @return The style (Text by default).
		 */
		Color getColorStyle(void) const noexcept;

		/**
		 * @brief Choose which theme colors the button uses.
		 * @param style The color style.
		 * @return This component, for chaining.
		 */
		CommonButton &setColorStyle(Color style);

		/**
		 * @brief Get the corner shape at rest.
		 * @return The shape (Round by default).
		 */
		Shape getShape(void) const noexcept;

		/**
		 * @brief Choose the corner shape at rest.
		 *
		 * While pressed the button takes the pressed radius of its size
		 * whatever the shape, and a selected toggle with
		 * Button::setMorph(true) draws the opposite shape.
		 * @param shape Round or Square.
		 * @return This component, for chaining.
		 */
		CommonButton &setShape(Shape shape);

		/**
		 * @brief Get the size step.
		 * @return The size (Small by default).
		 */
		Size getSize(void) const noexcept;

		/**
		 * @brief Choose the size step.
		 * @param size The size; it drives padding, icon size and radius.
		 * @return This component, for chaining.
		 */
		CommonButton &setSize(Size size);

		/**
		 * @brief Check whether the horizontal padding is halved.
		 * @return True when compact.
		 */
		bool isCompact(void) const noexcept;

		/**
		 * @brief Halve the horizontal padding, or restore it.
		 * @param compact True for half the horizontal padding of the size.
		 * @return This component, for chaining.
		 */
		CommonButton &setCompact(bool compact);

		/**
		 * @brief Get the requested minimum width.
		 * @return The width, in dp; 0 when only MinWidth applies.
		 */
		float getMinWidth(void) const noexcept;

		/**
		 * @brief Request a minimum container width.
		 *
		 * The container is as wide as the largest of its padded content,
		 * MinWidth and this value.
		 * @param width The minimum width, in dp.
		 * @return This component, for chaining.
		 */
		CommonButton &setMinWidth(float width);

		/**
		 * @brief Check whether the label is drawn in upper case.
		 * @return True when the label is upper-cased.
		 */
		bool isUppercase(void) const noexcept;

		/**
		 * @brief Draw the label in upper case, or as written.
		 *
		 * The text stored in components::Button is left as written; only
		 * the label child receives the upper-cased copy.
		 * @param uppercase True to upper-case the label.
		 * @return This component, for chaining.
		 */
		CommonButton &setUppercase(bool uppercase);

		/**
		 * @brief Get the per-corner radius override.
		 * @return The corners, or std::nullopt when the shape decides the
		 * radius.
		 */
		const std::optional<Corners> &getCorners(void) const noexcept;

		/**
		 * @brief Override the radius of each corner.
		 *
		 * systems::SegmentedButton sets it on each segment so that only the
		 * outer corners of the group are rounded.
		 * @param corners The corners, or std::nullopt to let the shape
		 * decide.
		 * @return This component, for chaining.
		 */
		CommonButton &setCorners(const std::optional<Corners> &corners);
	};

}	 // namespace guillaume::components
