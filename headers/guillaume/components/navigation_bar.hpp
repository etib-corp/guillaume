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

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief State of a navigation bar: the bottom strip of three to five
	 * destinations that switches between the top-level views of an app.
	 *
	 * The entities::NavigationBar widget writes it: the destination data,
	 * the callbacks, and the identifier of the destination child it builds
	 * for each entry. systems::NavigationBar reads it every frame in the
	 * Layout phase and turns it into the bar color, the width of each
	 * destination, their placement in a row and the visibility of each
	 * label.
	 *
	 * Which destination is selected is not stored here: it is the
	 * components::Selectable of each destination child. Read it with
	 * systems::NavigationBar::selectedIndex and change it with
	 * systems::NavigationBar::select.
	 *
	 * @see systems::NavigationBar, components::NavigationBarDestination,
	 * entities::NavigationBar
	 */
	class NavigationBar: public ecs::Component
	{
		public:
		/**
		 * @brief Whether every destination shows its label (WithLabels) or
		 * only the selected one does (WithoutLabels).
		 */
		enum class Variant { WithLabels, WithoutLabels };

		/**
		 * @brief Description of one destination, as given by the user.
		 */
		struct Destination {
			std::string icon;			///< Icon glyph name while not selected.
			std::string label;			///< Text under the icon.
			std::string focusedIcon;	///< Icon glyph name while selected;
										///< empty keeps `icon`.
			std::string badge;			///< Badge text; empty means no badge.

			/**
			 * @brief Compare two destinations.
			 * @param other The destination to compare with.
			 * @return True when every field is equal.
			 */
			bool operator==(const Destination &other) const = default;
		};

		/// Callback that receives the index of a destination, starting at 0.
		using IndexHandler = std::function<void(std::size_t)>;

		/// Height of the bar, in dp.
		static constexpr float Height = 80.0f;

		private:
		Variant _variant { Variant::WithLabels };	  ///< Label visibility
		std::vector<Destination> _destinations {};	  ///< Destination data
		std::vector<ecs::Entity::Identifier>
			_destinationEntities {};		  ///< Destination child of each
		IndexHandler _onIndexChange {};		  ///< Selection change callback
		IndexHandler _onTabPress {};		  ///< Destination press callback
		bool _updatingSelection { false };	  ///< Reentrancy guard

		public:
		/**
		 * @brief Create the configuration of a bar with labels and no
		 * destination.
		 */
		NavigationBar(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~NavigationBar(void) = default;

		/**
		 * @brief Get the label visibility rule.
		 * @return The variant, Variant::WithLabels by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Choose whether every destination shows its label.
		 *
		 * Marks the component changed only when the variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		NavigationBar &setVariant(Variant variant);

		/**
		 * @brief Get the description of every destination.
		 * @return The destinations, in display order.
		 */
		const std::vector<Destination> &getDestinations(void) const noexcept;

		/**
		 * @brief Replace the description of every destination.
		 *
		 * This does not build the destination children; the widget does.
		 * Marks the component changed only when the list differs.
		 * @param destinations The new destinations, in display order.
		 * @return This component, for chaining.
		 */
		NavigationBar &
			setDestinations(const std::vector<Destination> &destinations);

		/**
		 * @brief Append the description of one destination.
		 *
		 * Always marks the component changed.
		 * @param destination The destination to add after the existing
		 * ones.
		 * @return This component, for chaining.
		 */
		NavigationBar &addDestination(const Destination &destination);

		/**
		 * @brief Change the badge text recorded for one destination.
		 *
		 * This only updates the description. To update the badge on screen
		 * too, call systems::NavigationBar::setBadge. Marks the component
		 * changed only when the text differs.
		 * @param index The destination index, starting at 0. An index out
		 * of range is ignored.
		 * @param badge The new badge text.
		 * @return This component, for chaining.
		 */
		NavigationBar &setDestinationBadge(std::size_t index,
										   const std::string &badge);

		/**
		 * @brief Get the destination children.
		 * @return One identifier per destination, in the same order as
		 * getDestinations(). Empty until the widget has built them.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getDestinationEntities(void) const noexcept;

		/**
		 * @brief Record the destination children.
		 *
		 * Called by the widget after it creates them. Marks the component
		 * changed only when the list differs.
		 * @param identifiers One identifier per destination, in the same
		 * order as getDestinations().
		 * @return This component, for chaining.
		 */
		NavigationBar &setDestinationEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/**
		 * @brief Get the callback run when a press changes the selected
		 * destination.
		 * @return The callback; empty when none is set.
		 */
		const IndexHandler &getOnIndexChange(void) const noexcept;

		/**
		 * @brief Set the callback run when a press changes the selected
		 * destination.
		 *
		 * It receives the index of the newly selected destination. It is
		 * not called when the user presses the destination that is already
		 * selected. Does not mark the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		NavigationBar &setOnIndexChange(IndexHandler handler);

		/**
		 * @brief Get the callback run on every destination press.
		 * @return The callback; empty when none is set.
		 */
		const IndexHandler &getOnTabPress(void) const noexcept;

		/**
		 * @brief Set the callback run on every destination press.
		 *
		 * It receives the index of the pressed destination and runs before
		 * the selection changes, even when that destination is already
		 * selected. Does not mark the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		NavigationBar &setOnTabPress(IndexHandler handler);

		/**
		 * @brief Tell whether systems::NavigationBar is changing the
		 * selection at this moment.
		 * @return True during systems::NavigationBar::select.
		 */
		bool isUpdatingSelection(void) const noexcept;

		/**
		 * @brief Raise or lower the guard that stops a selection change
		 * from triggering another one.
		 *
		 * systems::NavigationBar raises it while it selects a destination,
		 * so the selection change the destination reports back is ignored.
		 * Marks the component changed when the flag differs.
		 * @param updating True while the system changes the selection.
		 * @return This component, for chaining.
		 */
		NavigationBar &setUpdatingSelection(bool updating);
	};

}	 // namespace guillaume::components
