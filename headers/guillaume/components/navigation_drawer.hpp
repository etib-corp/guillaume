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
	 * @brief State of a navigation drawer: the side sheet that lists the
	 * destinations of an app, grouped by section headers and dividers.
	 *
	 * The entities::NavigationDrawer widget writes it: the variant, the
	 * entries, the selection callback, and the identifiers of the children
	 * it builds for the entries and of the modal backdrop.
	 * systems::NavigationDrawer reads it every frame in the Layout phase and
	 * turns it into the sheet's size, shape and color, the column of
	 * entries, the flags of the drawer's components::Overlay and the
	 * backdrop.
	 *
	 * Two things are not stored here. Which destination is selected is the
	 * components::Selectable of each destination child (see
	 * systems::NavigationDrawer::selectedIndex). Whether the drawer is shown
	 * is its components::Overlay (see systems::NavigationDrawer::show).
	 *
	 * A *destination index* counts destinations only: sections and dividers
	 * are skipped. An *entry index* counts every entry.
	 *
	 * @see systems::NavigationDrawer,
	 * components::NavigationDrawerDestination, entities::NavigationDrawer
	 */
	class NavigationDrawer: public ecs::Component
	{
		public:
		/**
		 * @brief How the drawer is presented.
		 */
		enum class Variant {
			Standard,	 ///< Sheet docked beside the content, on `surface`.
			Modal,		 ///< Sheet on `surfaceContainerLow` above a backdrop
					  ///< that dims the content and closes it on a click.
			Collapsed	 ///< Narrow rail whose destinations stack an icon
						 ///< above a label.
		};

		/**
		 * @brief What a drawer entry is.
		 */
		enum class EntryKind {
			Destination,	///< Pressable destination.
			Section,		///< Section header (`titleSmall` text).
			Divider			///< Horizontal divider line.
		};

		/**
		 * @brief Description of one drawer entry, as given by the user.
		 *
		 * `icon`, `right` and `disabled` only apply to destinations.
		 */
		struct Entry {
			EntryKind kind { EntryKind::Destination };	  ///< What it is.
			std::string icon {};		///< Icon glyph name of a destination.
			std::string label {};		///< Destination label or section title.
			std::string right {};		///< Text at the trailing edge of a
										///< destination, such as a count.
			bool disabled { false };	///< True when the destination
										///< ignores presses.

			/**
			 * @brief Compare two entries.
			 * @param other The entry to compare with.
			 * @return True when every field is equal.
			 */
			bool operator==(const Entry &other) const = default;
		};

		/// Callback that receives a destination index, starting at 0.
		using IndexHandler = std::function<void(std::size_t)>;

		/// Width of a standard or modal drawer, in dp.
		static constexpr float Width = 320.0f;

		/// Width of a collapsed drawer (rail), in dp.
		static constexpr float CollapsedWidth = 80.0f;

		/// Height of the drawer, in dp.
		static constexpr float Height = 640.0f;

		private:
		Variant _variant { Variant::Standard };	   ///< Presentation
		std::vector<Entry> _entries {};			   ///< Drawer entries
		std::vector<ecs::Entity::Identifier>
			_entryEntities {};	  ///< Every entry child, in entry order
		std::vector<ecs::Entity::Identifier>
			_destinationEntities {};	///< Destination children
		std::vector<ecs::Entity::Identifier>
			_sectionEntities {};	///< Section header children
		std::vector<ecs::Entity::Identifier>
			_dividerEntities {};	///< Divider children
		ecs::Entity::Identifier _backdrop {
			ecs::Entity::InvalidIdentifier
		};	  ///< Modal backdrop child

		IndexHandler _onIndexChange {};		  ///< Selection change callback
		bool _updatingSelection { false };	  ///< Reentrancy guard

		public:
		/**
		 * @brief Create the configuration of a standard drawer with no
		 * entry.
		 */
		NavigationDrawer(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawer(void) = default;

		/**
		 * @brief Get how the drawer is presented.
		 * @return The variant, Variant::Standard by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Choose how the drawer is presented.
		 *
		 * Marks the component changed only when the variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		NavigationDrawer &setVariant(Variant variant);

		/**
		 * @brief Get the description of every entry.
		 * @return The entries, from top to bottom.
		 */
		const std::vector<Entry> &getEntries(void) const noexcept;

		/**
		 * @brief Replace the description of every entry.
		 *
		 * This does not build the entry children; the widget does. Marks
		 * the component changed only when the list differs.
		 * @param entries The new entries, from top to bottom.
		 * @return This component, for chaining.
		 */
		NavigationDrawer &setEntries(const std::vector<Entry> &entries);

		/**
		 * @brief Change the disabled flag recorded for one destination.
		 *
		 * This only updates the entry. To disable the destination on
		 * screen too, call systems::NavigationDrawer::setDestinationDisabled.
		 * Marks the component changed only when the flag differs.
		 * @param index The destination index, starting at 0. An index out
		 * of range is ignored.
		 * @param disabled True to disable the destination.
		 * @return This component, for chaining.
		 */
		NavigationDrawer &setDestinationDisabled(std::size_t index,
												 bool disabled);

		/**
		 * @brief Count the entries that are destinations.
		 * @return The number of entries of kind EntryKind::Destination.
		 */
		std::size_t getDestinationCount(void) const noexcept;

		/**
		 * @brief Get the child built for each entry, whatever its kind.
		 * @return The identifiers, in entry order. systems::NavigationDrawer
		 * places them in a column. Empty until the widget has built them.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getEntryEntities(void) const noexcept;

		/**
		 * @brief Record the child built for each entry, whatever its kind.
		 *
		 * Marks the component changed only when the list differs.
		 * @param identifiers The identifiers, in entry order.
		 * @return This component, for chaining.
		 */
		NavigationDrawer &setEntryEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/**
		 * @brief Get the destination children.
		 * @return The identifiers, indexed by destination index.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getDestinationEntities(void) const noexcept;

		/**
		 * @brief Record the destination children.
		 *
		 * Marks the component changed only when the list differs.
		 * @param identifiers The identifiers, indexed by destination index.
		 * @return This component, for chaining.
		 */
		NavigationDrawer &setDestinationEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/**
		 * @brief Get the section header children.
		 * @return One identifier per section entry that has a title, in
		 * entry order.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getSectionEntities(void) const noexcept;

		/**
		 * @brief Record the section header children.
		 *
		 * Marks the component changed only when the list differs.
		 * @param identifiers One identifier per section entry that has a
		 * title, in entry order.
		 * @return This component, for chaining.
		 */
		NavigationDrawer &setSectionEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/**
		 * @brief Get the divider children.
		 * @return Their identifiers, in entry order.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getDividerEntities(void) const noexcept;

		/**
		 * @brief Record the divider children.
		 *
		 * Marks the component changed only when the list differs.
		 * @param identifiers Their identifiers, in entry order.
		 * @return This component, for chaining.
		 */
		NavigationDrawer &setDividerEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/**
		 * @brief Get the child that dims the content behind a modal drawer.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getBackdrop(void) const noexcept;

		/**
		 * @brief Record the child that dims the content behind a modal
		 * drawer.
		 *
		 * Marks the component changed only when the identifier differs.
		 * @param identifier The backdrop's identifier.
		 * @return This component, for chaining.
		 */
		NavigationDrawer &setBackdrop(ecs::Entity::Identifier identifier);

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
		 * It receives the destination index of the newly selected
		 * destination. It is not called for a press on the destination
		 * that is already selected, nor on a disabled one. Does not mark
		 * the component changed.
		 * @param handler The callback; an empty one removes it.
		 * @return This component, for chaining.
		 */
		NavigationDrawer &setOnIndexChange(IndexHandler handler);

		/**
		 * @brief Tell whether systems::NavigationDrawer is changing the
		 * selection at this moment.
		 * @return True during systems::NavigationDrawer::select.
		 */
		bool isUpdatingSelection(void) const noexcept;

		/**
		 * @brief Raise or lower the guard that stops a selection change
		 * from triggering another one.
		 *
		 * systems::NavigationDrawer raises it while it selects a
		 * destination, so the selection change the destination reports back
		 * is ignored. Marks the component changed when the flag differs.
		 * @param updating True while the system changes the selection.
		 * @return This component, for chaining.
		 */
		NavigationDrawer &setUpdatingSelection(bool updating);
	};

}	 // namespace guillaume::components
