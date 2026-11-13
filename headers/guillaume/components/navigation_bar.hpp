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
	 * @brief Configuration of a navigation bar.
	 *
	 * This component is the whole state of a navigation bar: the variant, the
	 * data of each destination, the destination child of each of them, the
	 * callbacks and the transient selection guard. Which destination is
	 * selected is the components::Selectable of the destination children.
	 * systems::NavigationBar turns it into the bar color, the destination
	 * widths, the row placement and the label visibility of each destination.
	 */
	class NavigationBar: public ecs::Component
	{
		public:
		/**
		 * @brief Navigation bar variant.
		 */
		enum class Variant { WithLabels, WithoutLabels };

		/**
		 * @brief One navigation destination.
		 */
		struct Destination {
			std::string icon;			///< Unfocused (outlined) icon glyph.
			std::string label;			///< Label text.
			std::string focusedIcon;	///< Icon glyph while selected.
			std::string badge;			///< Badge label ("" hides it).

			/**
			 * @brief Compare two destinations.
			 * @param other The destination to compare with.
			 * @return True when every field is equal.
			 */
			bool operator==(const Destination &other) const = default;
		};

		/// Called with a destination index.
		using IndexHandler = std::function<void(std::size_t)>;

		/// M3 navigation bar height, in dp.
		static constexpr float Height = 80.0f;

		private:
		Variant _variant { Variant::WithLabels };	  ///< Bar variant
		std::vector<Destination> _destinations {};	  ///< Destination data
		std::vector<ecs::Entity::Identifier>
			_destinationEntities {};		  ///< Destination child of each
		IndexHandler _onIndexChange {};		  ///< Selection change callback
		IndexHandler _onTabPress {};		  ///< Destination press callback
		bool _updatingSelection { false };	  ///< Reentrancy guard

		public:
		/**
		 * @brief Default constructor.
		 */
		NavigationBar(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~NavigationBar(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		NavigationBar &setVariant(Variant variant);

		/** @brief Get the destination data. @return The destinations. */
		const std::vector<Destination> &getDestinations(void) const noexcept;

		/** @brief Set the destination data. @param destinations They. @return
		 * It. */
		NavigationBar &
			setDestinations(const std::vector<Destination> &destinations);

		/** @brief Append a destination. @param destination It. @return It. */
		NavigationBar &addDestination(const Destination &destination);

		/**
		 * @brief Record the badge label of one destination.
		 * @param index The destination index (ignored when out of range).
		 * @param badge The badge label.
		 * @return It.
		 */
		NavigationBar &setDestinationBadge(std::size_t index,
										   const std::string &badge);

		/** @brief Get the destination children. @return Their identifiers, in
		 * destination order. */
		const std::vector<ecs::Entity::Identifier> &
			getDestinationEntities(void) const noexcept;

		/** @brief Set the destination children. @param identifiers Their
		 * identifiers, in destination order. @return It. */
		NavigationBar &setDestinationEntities(
			const std::vector<ecs::Entity::Identifier> &identifiers);

		/** @brief Get the selection change callback. @return The callback. */
		const IndexHandler &getOnIndexChange(void) const noexcept;

		/** @brief Set the selection change callback. @param handler It.
		 * @return It. */
		NavigationBar &setOnIndexChange(IndexHandler handler);

		/** @brief Get the destination press callback. @return The callback. */
		const IndexHandler &getOnTabPress(void) const noexcept;

		/** @brief Set the destination press callback. @param handler It.
		 * @return It. */
		NavigationBar &setOnTabPress(IndexHandler handler);

		/** @brief Whether the bar is changing the selection itself. @return
		 * True while it is. */
		bool isUpdatingSelection(void) const noexcept;

		/** @brief Set whether the bar is changing the selection itself.
		 * @param updating It. @return It. */
		NavigationBar &setUpdatingSelection(bool updating);
	};

}	 // namespace guillaume::components
