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

#include <vector>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief How a container arranges its children: in a row or a column,
	 * with which alignment, spacing and padding, and at which size.
	 *
	 * The container's widget class (entities::View and the widgets built
	 * on it) or the application writes it. systems::Layout reads it every
	 * frame in the Layout phase and then:
	 * - reads the components::Bound of each child of getChildren();
	 * - writes the position of each child's components::Transform, next
	 *   to one another along the axis and pushed towards the viewer so
	 *   the children are drawn in front of the container;
	 * - writes the container's own components::Bound: the fixed size when
	 *   one is set, otherwise the size of the children plus the padding.
	 *
	 * @code
	 * // A 320 dp wide column, 8 dp between children, 16 dp around them.
	 * registry.getComponent<components::Layout>(container)
	 * 	.setAxis(components::Layout::Axis::Vertical)
	 * 	.setSpacing(8.0f)
	 * 	.setPadding(16.0f)
	 * 	.setFixedWidth(320.0f)
	 * 	.setChildren({ title, body, button });
	 * @endcode
	 *
	 * @note The main axis is the one of setAxis(); the cross axis is the
	 * other one. For a row, main is X and cross is Y.
	 * @note The size of a child is never changed: a layout only moves its
	 * children. A child without a Transform or a Bound, or equal to
	 * ecs::Entity::InvalidIdentifier, is skipped.
	 * @note The list of children is separate from the entity tree
	 * (components::Hierarchy): only the listed children are arranged, in
	 * the order of the list.
	 *
	 * @see systems::Layout, components::Bound, components::Hierarchy
	 */
	class Layout: public ecs::Component
	{
		public:
		/**
		 * @brief Direction along which children are arranged.
		 */
		enum class Axis {
			Horizontal,	   ///< Row: children placed left to right.
			Vertical	   ///< Column: children placed top to bottom.
		};

		/**
		 * @brief Where the children go along the main axis when the
		 * container is larger than them.
		 *
		 * It only shows with a fixed size on the main axis: a container
		 * that wraps its children has no room left to distribute.
		 */
		enum class MainAxisAlignment {
			Start,			///< Packed at the left (row) or top (column).
			Center,			///< Packed in the middle.
			End,			///< Packed at the right (row) or bottom (column).
			SpaceBetween	///< First and last at the edges, equal gaps
							///< between; the spacing is ignored.
		};

		/**
		 * @brief Where each child goes along the cross axis, inside the
		 * padding. In a container that wraps its children, the reference
		 * is the largest child.
		 */
		enum class CrossAxisAlignment {
			Start,	   ///< At the top (row) or left (column).
			Center,	   ///< In the middle.
			End		   ///< At the bottom (row) or right (column).
		};

		private:
		Axis _axis { Axis::Horizontal };	///< Main axis.
		MainAxisAlignment _mainAxisAlignment {
			MainAxisAlignment::Start
		};	  ///< Alignment along the main axis.

		CrossAxisAlignment _crossAxisAlignment {
			CrossAxisAlignment::Center
		};	  ///< Alignment along the cross axis.

		float _spacing { 0.0f };		   ///< Gap between children, in dp.
		float _padding { 0.0f };		   ///< Inset on the four sides, in dp.
		bool _hasFixedWidth { false };	   ///< Whether the width is forced.
		bool _hasFixedHeight { false };	   ///< Whether the height is forced.
		float _fixedWidth { 0.0f };		   ///< Forced container width, in dp.
		float _fixedHeight { 0.0f };	   ///< Forced container height, in dp.
		std::vector<ecs::Entity::Identifier>
			_children {};	 ///< Children to arrange, in order.

		public:
		/**
		 * @brief Default constructor for the Layout component.
		 */
		Layout(void) = default;

		/**
		 * @brief Default destructor for the Layout component.
		 */
		~Layout(void) = default;

		/**
		 * @brief Get the main axis.
		 * @return Horizontal for a row (the default), Vertical for a
		 * column.
		 */
		Axis getAxis(void) const;

		/**
		 * @brief Choose between a row and a column.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param axis Horizontal for a row, Vertical for a column.
		 * @return This component, for chaining.
		 */
		Layout &setAxis(Axis axis);

		/**
		 * @brief Get the alignment along the main axis.
		 * @return The alignment (Start by default).
		 */
		MainAxisAlignment getMainAxisAlignment(void) const;

		/**
		 * @brief Set where the children go along the main axis.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param alignment The alignment; see MainAxisAlignment for when
		 * it has a visible effect.
		 * @return This component, for chaining.
		 */
		Layout &setMainAxisAlignment(MainAxisAlignment alignment);

		/**
		 * @brief Get the alignment along the cross axis.
		 * @return The alignment (Center by default).
		 */
		CrossAxisAlignment getCrossAxisAlignment(void) const;

		/**
		 * @brief Set where each child goes along the cross axis.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param alignment The alignment of every child.
		 * @return This component, for chaining.
		 */
		Layout &setCrossAxisAlignment(CrossAxisAlignment alignment);

		/**
		 * @brief Get the gap between two consecutive children.
		 * @return The spacing, in dp (0 by default).
		 */
		float getSpacing(void) const;

		/**
		 * @brief Set the gap between two consecutive children.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param spacing The gap along the main axis, in dp. Ignored with
		 * MainAxisAlignment::SpaceBetween.
		 * @return This component, for chaining.
		 */
		Layout &setSpacing(float spacing);

		/**
		 * @brief Get the inset between the container's edges and its
		 * children.
		 * @return The padding, in dp, the same on the four sides (0 by
		 * default).
		 */
		float getPadding(void) const;

		/**
		 * @brief Set the inset between the container's edges and its
		 * children.
		 *
		 * Marks the component changed only when the value differs.
		 *
		 * @param padding The padding, in dp, applied to the four sides.
		 * @return This component, for chaining.
		 */
		Layout &setPadding(float padding);

		/**
		 * @brief Check whether the container width is forced.
		 * @return True after setFixedWidth(); false when the width wraps
		 * the children (the default).
		 */
		bool hasFixedWidth(void) const;

		/**
		 * @brief Get the forced container width.
		 * @return The width, in dp, padding included; 0 when
		 * hasFixedWidth() is false.
		 */
		float getFixedWidth(void) const;

		/**
		 * @brief Force the container width instead of wrapping the
		 * children.
		 *
		 * systems::Layout then writes this width into the container's
		 * Bound, whatever the children need: children that do not fit
		 * overflow. Marks the component changed unless the same width was
		 * already forced.
		 *
		 * @param width The width of the container, in dp, padding
		 * included.
		 * @return This component, for chaining.
		 */
		Layout &setFixedWidth(float width);

		/**
		 * @brief Let the width wrap the children again.
		 *
		 * Marks the component changed only when a width was forced.
		 *
		 * @return This component, for chaining.
		 */
		Layout &clearFixedWidth(void);

		/**
		 * @brief Check whether the container height is forced.
		 * @return True after setFixedHeight(); false when the height wraps
		 * the children (the default).
		 */
		bool hasFixedHeight(void) const;

		/**
		 * @brief Get the forced container height.
		 * @return The height, in dp, padding included; 0 when
		 * hasFixedHeight() is false.
		 */
		float getFixedHeight(void) const;

		/**
		 * @brief Force the container height instead of wrapping the
		 * children.
		 *
		 * systems::Layout then writes this height into the container's
		 * Bound, whatever the children need. Marks the component changed
		 * unless the same height was already forced.
		 *
		 * @param height The height of the container, in dp, padding
		 * included.
		 * @return This component, for chaining.
		 */
		Layout &setFixedHeight(float height);

		/**
		 * @brief Let the height wrap the children again.
		 *
		 * Marks the component changed only when a height was forced.
		 *
		 * @return This component, for chaining.
		 */
		Layout &clearFixedHeight(void);

		/**
		 * @brief Get the children systems::Layout arranges, in order.
		 *
		 * Children the container's own system places (a switch thumb, a
		 * slider fill, ...) are left out of this list on purpose and keep
		 * the pose that system gives them.
		 *
		 * @return The child identifiers; empty when nothing is arranged.
		 */
		const std::vector<ecs::Entity::Identifier> &getChildren(void) const;

		/**
		 * @brief Replace the list of children to arrange.
		 *
		 * Marks the component changed only when the list differs.
		 *
		 * @param children The child identifiers, first to last along the
		 * main axis.
		 * @return This component, for chaining.
		 */
		Layout &
			setChildren(const std::vector<ecs::Entity::Identifier> &children);

		/**
		 * @brief Add a child at the end of the list.
		 *
		 * Always marks the component changed. The identifier is not
		 * checked: adding the same child twice lists it twice.
		 *
		 * @param child The identifier of the child to arrange last.
		 * @return This component, for chaining.
		 */
		Layout &addChild(ecs::Entity::Identifier child);

		/**
		 * @brief Empty the list of children; the entities themselves are
		 * left untouched.
		 *
		 * Marks the component changed only when the list was not empty.
		 *
		 * @return This component, for chaining.
		 */
		Layout &clearChildren(void);
	};

}	 // namespace guillaume::components
