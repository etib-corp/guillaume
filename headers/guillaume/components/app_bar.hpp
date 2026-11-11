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

#include <functional>
#include <string>
#include <vector>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::components
{

	/**
	 * @brief Configuration of an app bar.
	 *
	 * This component is the whole state of an app bar. systems::AppBar turns
	 * it into the bar's Layout, Color, Elevation and Bound, and into the
	 * content and arrangement of its navigation, title and action children.
	 */
	class AppBar: public ecs::Component
	{
		public:
		/**
		 * @brief App bar variant.
		 */
		enum class Variant { Small, CenterAligned, Medium, Large, Bottom };

		/**
		 * @brief A pressable app bar icon (navigation or action).
		 */
		struct Action {
			std::string icon {};					 ///< Icon glyph.
			std::function<void(void)> onPress {};	 ///< Press handler.
			std::string accessibilityLabel {};		 ///< Accessible name.
			bool disabled { false };				 ///< Whether disabled.
		};

		/// Child identifiers.
		using Identifiers = std::vector<ecs::Entity::Identifier>;

		/**
		 * @brief Horizontal padding (Paper: 4 dp).
		 */
		static constexpr float HorizontalPadding = 4.0f;

		private:
		Variant _variant { Variant::Small };	///< App bar variant
		std::string _title {};					///< Title text
		Action _navigation {};					///< Leading navigation action
		std::vector<Action> _actions {};		///< Trailing actions
		bool _elevated { false };				///< Level-2 tonal elevation
		ecs::Entity::Identifier _navigationButton {
			ecs::Entity::InvalidIdentifier
		};	  ///< Leading icon button child
		ecs::Entity::Identifier _titleEntity {
			ecs::Entity::InvalidIdentifier
		};	  ///< Title child
		Identifiers _actionButtons {};	  ///< Trailing action children

		public:
		/**
		 * @brief Default constructor.
		 */
		AppBar(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~AppBar(void) = default;

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const noexcept;

		/** @brief Set the variant. @param variant It. @return It. */
		AppBar &setVariant(Variant variant);

		/** @brief Get the title text. @return The title. */
		const std::string &getTitle(void) const noexcept;

		/** @brief Set the title text. @param title It. @return It. */
		AppBar &setTitle(const std::string &title);

		/** @brief Get the navigation action. @return The action. */
		const Action &getNavigation(void) const noexcept;

		/**
		 * @brief Set the navigation action (an empty icon means none).
		 * @param navigation The new action.
		 * @return Reference to this component for chaining.
		 */
		AppBar &setNavigation(Action navigation);

		/** @brief Get the trailing actions. @return The actions. */
		const std::vector<Action> &getActions(void) const noexcept;

		/**
		 * @brief Set the trailing actions.
		 * @param actions The new actions.
		 * @return Reference to this component for chaining.
		 */
		AppBar &setActions(std::vector<Action> actions);

		/** @brief Whether the bar is elevated. @return True if so. */
		bool isElevated(void) const noexcept;

		/** @brief Set the elevated flag. @param elevated It. @return It. */
		AppBar &setElevated(bool elevated);

		/** @brief Get the navigation button child. @return Its identifier. */
		ecs::Entity::Identifier getNavigationButton(void) const noexcept;

		/** @brief Set the navigation button child. @param identifier It.
		 * @return It. */
		AppBar &setNavigationButton(ecs::Entity::Identifier identifier);

		/** @brief Get the title child. @return Its identifier. */
		ecs::Entity::Identifier getTitleEntity(void) const noexcept;

		/** @brief Set the title child. @param identifier It. @return It. */
		AppBar &setTitleEntity(ecs::Entity::Identifier identifier);

		/** @brief Get the action button children. @return Their identifiers.
		 */
		const Identifiers &getActionButtons(void) const noexcept;

		/** @brief Set the action button children. @param identifiers They.
		 * @return It. */
		AppBar &setActionButtons(const Identifiers &identifiers);
	};

}	 // namespace guillaume::components
