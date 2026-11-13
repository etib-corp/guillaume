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
	 * @brief Configuration of a navigation drawer.
	 *
	 * This component is the whole state of a navigation drawer: the variant,
	 * the entries, the children built for them (destinations, section
	 * headers, dividers, and all of them in entry order), the modal backdrop,
	 * the selection callback and the transient selection guard. Which
	 * destination is selected is the components::Selectable of the
	 * destination children; whether the drawer is shown is its
	 * components::Overlay. systems::NavigationDrawer turns it into the sheet
	 * geometry and colors, the column placement, the overlay flags and the
	 * backdrop.
	 */
	class NavigationDrawer: public ecs::Component
	{
		public:
		/**
		 * @brief Drawer variant.
		 */
		enum class Variant {
			Standard,	 ///< Docked drawer on `surface`.
			Modal,		 ///< Drawer on `surfaceContainerLow` over a backdrop.
			Collapsed	 ///< Narrow rail of collapsed items.
		};

		/**
		 * @brief Kind of a drawer entry.
		 */
		enum class EntryKind {
			Destination,	///< Pressable destination.
			Section,		///< Section header (`titleSmall`).
			Divider			///< Horizontal divider.
		};

		/**
		 * @brief One drawer entry.
		 */
		struct Entry {
			EntryKind kind { EntryKind::Destination };	  ///< Entry kind.
			std::string icon {};		///< Destination icon glyph.
			std::string label {};		///< Destination label or section title.
			std::string right {};		///< Trailing text (badge count, ...).
			bool disabled { false };	///< Whether the destination is
										///< disabled.

			/**
			 * @brief Compare two entries.
			 * @param other The entry to compare with.
			 * @return True when every field is equal.
			 */
			bool operator==(const Entry &other) const = default;
		};

		/// Called with a destination index.
		using IndexHandler = std::function<void(std::size_t)>;

		/// M3 standard and modal drawer width, in dp.
		static constexpr float Width = 320.0f;

		/// Collapsed drawer (rail) width, in dp.
		static constexpr float CollapsedWidth = 80.0f;

		/// Drawer height, in dp.
		static constexpr float Height = 640.0f;

		private:
		Variant _variant { Variant::Standard };	   ///< Drawer variant
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
		 * @brief Default constructor.
		 */
		NavigationDrawer(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawer(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		NavigationDrawer &setVariant(Variant variant);

		/** @brief Get the entries. @return The entries. */
		const std::vector<Entry> &getEntries(void) const noexcept;

		/** @brief Set the entries. @param entries They. @return It. */
		NavigationDrawer &setEntries(const std::vector<Entry> &entries);

		/**
		 * @brief Record whether one destination is disabled.
		 * @param index The destination index: sections and dividers are not
		 * counted (ignored when out of range).
		 * @param disabled True when disabled.
		 * @return It.
		 */
		NavigationDrawer &setDestinationDisabled(std::size_t index,
												 bool disabled);

		/** @brief Get the number of destination entries. @return The count.
		 */
		std::size_t getDestinationCount(void) const noexcept;

		/** @brief Get every entry child. @return Their identifiers, in entry
		 * order. */
		const std::vector<ecs::Entity::Identifier> &
			getEntryEntities(void) const noexcept;

		/** @brief Set every entry child. @param identifiers Their
		 * identifiers, in entry order. @return It. */
		NavigationDrawer &setEntryEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/** @brief Get the destination children. @return Their identifiers, in
		 * destination order. */
		const std::vector<ecs::Entity::Identifier> &
			getDestinationEntities(void) const noexcept;

		/** @brief Set the destination children. @param identifiers Their
		 * identifiers, in destination order. @return It. */
		NavigationDrawer &setDestinationEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/** @brief Get the section header children. @return Their identifiers.
		 */
		const std::vector<ecs::Entity::Identifier> &
			getSectionEntities(void) const noexcept;

		/** @brief Set the section header children. @param identifiers They.
		 * @return It. */
		NavigationDrawer &setSectionEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/** @brief Get the divider children. @return Their identifiers. */
		const std::vector<ecs::Entity::Identifier> &
			getDividerEntities(void) const noexcept;

		/** @brief Set the divider children. @param identifiers They. @return
		 * It. */
		NavigationDrawer &setDividerEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/** @brief Get the modal backdrop child. @return Its identifier. */
		ecs::Entity::Identifier getBackdrop(void) const noexcept;

		/** @brief Set the modal backdrop child. @param identifier It. @return
		 * It. */
		NavigationDrawer &setBackdrop(ecs::Entity::Identifier identifier);

		/** @brief Get the selection change callback. @return The callback. */
		const IndexHandler &getOnIndexChange(void) const noexcept;

		/** @brief Set the selection change callback. @param handler It.
		 * @return It. */
		NavigationDrawer &setOnIndexChange(IndexHandler handler);

		/** @brief Whether the drawer is changing the selection itself.
		 * @return True while it is. */
		bool isUpdatingSelection(void) const noexcept;

		/** @brief Set whether the drawer is changing the selection itself.
		 * @param updating It. @return It. */
		NavigationDrawer &setUpdatingSelection(bool updating);
	};

}	 // namespace guillaume::components
