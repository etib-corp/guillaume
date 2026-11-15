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
	 * @brief State of an app bar: the strip that carries a screen's title,
	 * its navigation icon and its action icons.
	 *
	 * The entities::AppBar widget writes it: its setters store the variant,
	 * title and actions here, and its build step records the identifiers of
	 * the children it creates (one icon button per action, one text for the
	 * title). systems::AppBar reads it every frame in the Layout phase and
	 * turns it into the bar's Layout, Color, Elevation and Bound, and into
	 * the text and typescale role of the title child.
	 *
	 * The component only describes the bar. Changing the navigation action
	 * or the actions here does not create or remove icon buttons; the widget
	 * does that. Use the widget's setters unless you drive the system on a
	 * bare identifier.
	 *
	 * @see systems::AppBar, entities::AppBar
	 */
	class AppBar: public ecs::Component
	{
		public:
		/**
		 * @brief Shape of the bar. It sets the height and the title size.
		 *
		 * Small and CenterAligned are 64 dp high, Medium 112 dp, Large
		 * 152 dp and Bottom 80 dp. CenterAligned also centers the title
		 * (or spreads the children when there are icons). Bottom is always
		 * drawn with the elevated surface color.
		 */
		enum class Variant { Small, CenterAligned, Medium, Large, Bottom };

		/**
		 * @brief One pressable icon of the bar: the leading navigation icon
		 * or a trailing action.
		 */
		struct Action {
			std::string icon {};	///< Icon glyph name; empty means no icon.
			std::function<void(void)> onPress {};	 ///< Called on press.
			std::string accessibilityLabel {};		 ///< Name read by a screen
													 ///< reader.
			bool disabled { false };	///< True when the icon ignores input.
		};

		/// Identifiers of child entities, in display order.
		using Identifiers = std::vector<ecs::Entity::Identifier>;

		/**
		 * @brief Padding on each side of the bar and gap between its
		 * children, in dp.
		 */
		static constexpr float HorizontalPadding = 4.0f;

		private:
		Variant _variant { Variant::Small };	///< Shape of the bar
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
		 * @brief Create the configuration of a small, flat bar with no title
		 * and no icon.
		 */
		AppBar(void) = default;

		/**
		 * @brief Default destructor.
		 */
		~AppBar(void) = default;

		/**
		 * @brief Get the shape of the bar.
		 * @return The variant, Variant::Small by default.
		 */
		Variant getVariant(void) const noexcept;

		/**
		 * @brief Set the shape of the bar, which decides its height and the
		 * size of its title.
		 *
		 * Marks the component changed only when the variant differs.
		 * @param variant The new variant.
		 * @return This component, for chaining.
		 */
		AppBar &setVariant(Variant variant);

		/**
		 * @brief Get the title shown in the bar.
		 * @return The title text; empty when the bar has no title.
		 */
		const std::string &getTitle(void) const noexcept;

		/**
		 * @brief Set the title shown in the bar.
		 *
		 * systems::AppBar copies it to the title child, when that child
		 * exists. Marks the component changed only when the text differs.
		 * @param title The new title text.
		 * @return This component, for chaining.
		 */
		AppBar &setTitle(const std::string &title);

		/**
		 * @brief Get the leading navigation action (back arrow, menu icon).
		 * @return The action; its icon is empty when the bar has none.
		 */
		const Action &getNavigation(void) const noexcept;

		/**
		 * @brief Set the leading navigation action.
		 *
		 * Always marks the component changed, because the callback of an
		 * action cannot be compared.
		 * @param navigation The new action; an empty icon means no
		 * navigation icon.
		 * @return This component, for chaining.
		 */
		AppBar &setNavigation(Action navigation);

		/**
		 * @brief Get the trailing actions.
		 * @return The actions, in display order (leading edge first).
		 */
		const std::vector<Action> &getActions(void) const noexcept;

		/**
		 * @brief Replace the trailing actions.
		 *
		 * Always marks the component changed, because the callback of an
		 * action cannot be compared.
		 * @param actions The new actions, in display order.
		 * @return This component, for chaining.
		 */
		AppBar &setActions(std::vector<Action> actions);

		/**
		 * @brief Tell whether the bar is elevated.
		 * @return True when the bar uses the level-2 surface color and
		 * elevation; false when it is flat.
		 */
		bool isElevated(void) const noexcept;

		/**
		 * @brief Raise or flatten the bar.
		 *
		 * An elevated bar gets the level-2 tinted surface color and the
		 * level-2 elevation. Marks the component changed only when the flag
		 * differs.
		 * @param elevated True to elevate the bar.
		 * @return This component, for chaining.
		 */
		AppBar &setElevated(bool elevated);

		/**
		 * @brief Get the icon button child built for the navigation action.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getNavigationButton(void) const noexcept;

		/**
		 * @brief Record the icon button child built for the navigation
		 * action.
		 *
		 * Called by the widget after it creates the child. Marks the
		 * component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		AppBar &setNavigationButton(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the text child that displays the title.
		 * @return Its identifier, or ecs::Entity::InvalidIdentifier when it
		 * has not been built.
		 */
		ecs::Entity::Identifier getTitleEntity(void) const noexcept;

		/**
		 * @brief Record the text child that displays the title.
		 *
		 * Called by the widget after it creates the child. Marks the
		 * component changed only when the identifier differs.
		 * @param identifier The child's identifier.
		 * @return This component, for chaining.
		 */
		AppBar &setTitleEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Get the icon button children built for the trailing
		 * actions.
		 * @return One identifier per action, in the same order; an entry is
		 * ecs::Entity::InvalidIdentifier when its button could not be built.
		 */
		const Identifiers &getActionButtons(void) const noexcept;

		/**
		 * @brief Record the icon button children built for the trailing
		 * actions.
		 *
		 * systems::AppBar considers the children missing when this list and
		 * the actions differ in size. Marks the component changed only when
		 * the list differs.
		 * @param identifiers One identifier per action, in the same order.
		 * @return This component, for chaining.
		 */
		AppBar &setActionButtons(const Identifiers &identifiers);
	};

}	 // namespace guillaume::components
