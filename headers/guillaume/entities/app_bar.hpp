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

#include <utility>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/app_bar.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/icon_button.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A horizontal bar with a leading navigation icon button, a
	 * title and trailing action icon buttons.
	 *
	 * Its state lives in components::AppBar (variant, title, navigation
	 * action, trailing actions, elevated flag, child identifiers);
	 * systems::AppBar styles the bar and arranges its children each frame,
	 * in the Layout phase. The class itself holds no field: its setters
	 * write the component and restyle at once.
	 *
	 * @code
	 * AppBar::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto appBar = builder.withTitle("Inbox")
	 * 	.withNavigationIcon("menu")
	 * 	.withOnNavigation([] {
	 * 		// called when the user presses the navigation icon
	 * 	})
	 * 	.addAction("search", [] {}, "Search")
	 * 	.makeAppBar(parent, AppBar::Variant::Small);
	 * appBar->setTitle("Archive");
	 * @endcode
	 *
	 * `AppBar::create(registry, config, variant, title, navigation, actions,
	 * elevated)` builds the same widget without registering it in an entity
	 * registry.
	 *
	 * Things to know:
	 * - The variant fixes the height: 64 dp (`Small`, `CenterAligned`),
	 *   112 dp (`Medium`), 152 dp (`Large`) or 80 dp (`Bottom`). The width
	 *   is the fixed width of the surface configuration (360 dp with the
	 *   Builder defaults).
	 * - systems::AppBar overwrites each frame the bar's layout (axis,
	 *   alignments, padding, spacing, fixed height, children), its fill
	 *   color and its elevation, as well as the title's content and
	 *   typescale role and the navigation button's content color.
	 * - The app bar is the entity registry that owns its children: the
	 *   navigation button, the title and the action buttons.
	 * - The navigation action and the trailing actions are chosen at
	 *   creation; the class has no setter for them. The navigation button
	 *   exists only when a navigation icon is set.
	 *
	 * @see components::AppBar, systems::AppBar, IconButton
	 */
	class AppBar: public SurfaceBase<components::Elevation, components::AppBar>
	{
		public:
		/// Variant: it sets the height, the title typescale role and how
		/// the children are spread.
		using Variant = components::AppBar::Variant;

		/// A pressable app bar icon (navigation or action): glyph name,
		/// press handler, accessible name and disabled flag.
		using Action = components::AppBar::Action;

		/**
		 * @brief Configures an AppBar, creates it and registers it in an
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<AppBar>
		{
			private:
			SurfaceConfig _config;					///< Surface configuration.
			Variant _variant { Variant::Small };	///< App bar variant.
			std::string _title {};					///< Title text.
			Action _navigation {};				///< Leading navigation action.
			std::vector<Action> _actions {};	///< Trailing actions.
			bool _elevated { false };			///< Level-2 tonal elevation.

			public:
			/**
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry that stores the
			 * components of the app bars built.
			 * @param entityRegistry The registry the app bars are added to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build an app bar and its children from the current
			 * configuration, without registering it or setting its parent.
			 * @return The new app bar.
			 */
			std::shared_ptr<AppBar> buildEntity(void) override;

			/**
			 * @brief Restore the defaults.
			 *
			 * Defaults: `Small` variant, no title, no navigation icon, no
			 * action, not elevated, default pose, 360 dp wide.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the app bar is placed.
			 * @param pose The pose of the bar. Default: a
			 * default-constructed pose.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the variant.
			 * @param variant The variant; it sets the height and the title
			 * typescale role. Default: `Small`.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the title text.
			 * @param title The title; an empty string (the default) builds
			 * no title child.
			 * @return This builder, for chaining.
			 */
			Builder &withTitle(const std::string &title);

			/**
			 * @brief Set the icon of the leading navigation button.
			 *
			 * The glyph name also becomes the button's accessible name when
			 * none has been set with withOnNavigation().
			 * @param iconGlyphName The glyph name of the icon, such as
			 * `"menu"`. An empty name (the default) builds no navigation
			 * button.
			 * @return This builder, for chaining.
			 */
			Builder &withNavigationIcon(const std::string &iconGlyphName);

			/**
			 * @brief Append a trailing action button without press handler.
			 *
			 * The glyph name is also used as the accessible name.
			 * @param iconGlyphName The glyph name of the icon.
			 * @return This builder, for chaining.
			 */
			Builder &addAction(const std::string &iconGlyphName);

			/**
			 * @brief Append a trailing action button with a press handler
			 * (Paper `Appbar.Action`).
			 * @param iconGlyphName The glyph name of the icon.
			 * @param onPress Called when the user presses the button.
			 * @param accessibilityLabel The accessible name of the button;
			 * the glyph name is used when empty.
			 * @return This builder, for chaining.
			 */
			Builder &addAction(const std::string &iconGlyphName,
							   std::function<void(void)> onPress,
							   const std::string &accessibilityLabel = "");

			/**
			 * @brief Set the press handler of the navigation button (Paper
			 * `Appbar.BackAction`).
			 *
			 * The button is built only when withNavigationIcon() gave it an
			 * icon.
			 * @param onPress Called when the user presses the navigation
			 * button.
			 * @param accessibilityLabel The accessible name of the button.
			 * @return This builder, for chaining.
			 */
			Builder &withOnNavigation(
				std::function<void(void)> onPress,
				const std::string &accessibilityLabel = "Navigate up");

			/**
			 * @brief Choose the elevated look (Paper `elevated`).
			 * @param elevated True for the level-2 elevation and its tinted
			 * surface color, false (the default) for a flat bar.
			 * @return This builder, for chaining.
			 */
			Builder &withElevated(bool elevated);

			/**
			 * @brief Set the width of the app bar.
			 * @param width The width, in dp. Default: 360.
			 * @return This builder, for chaining.
			 */
			Builder &withFixedWidth(float width);

			/**
			 * @brief Build an app bar, register it under `parent` and reset
			 * the builder.
			 *
			 * Uses the title, navigation, actions and other options set on
			 * the builder so far.
			 * @param parent The entity the app bar becomes a child of, or
			 * nullptr for a root entity.
			 * @param variant The variant of the app bar.
			 * @return The new app bar.
			 */
			std::shared_ptr<AppBar>
				makeAppBar(std::shared_ptr<ecs::Entity> parent,
						   Variant variant);
		};

		/**
		 * @brief Create an app bar and its children, without registering it
		 * in an entity registry.
		 *
		 * The constructors are private because the children need the app
		 * bar to be owned by a shared pointer before they can take it as
		 * parent; create() constructs the app bar, then builds the
		 * navigation button, the title and the action buttons.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments Forwarded to a private constructor: either
		 * (registry, config, variant, title, navigation, actions, elevated)
		 * with `Action` values, or (registry, config, variant, title,
		 * navigationIcon, actions) with glyph names only (no press handler,
		 * not elevated).
		 * @return The new app bar.
		 */
		template<typename... Arguments>
		static std::shared_ptr<AppBar> create(Arguments &&...arguments)
		{
			std::shared_ptr<AppBar> entity(
				new AppBar(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~AppBar(void) override;

		/**
		 * @brief Change the variant.
		 *
		 * Restyles at once: the height, the title typescale role and the
		 * main-axis alignment follow the variant.
		 * @param variant The new variant.
		 * @return This app bar, for chaining.
		 */
		AppBar &setVariant(Variant variant);

		/**
		 * @brief Get the variant.
		 * @return The variant stored in components::AppBar.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Change the title text.
		 *
		 * Builds the title child when the app bar has none and `title` is
		 * not empty, then restyles at once. An empty title keeps an existing
		 * child, with an empty content.
		 * @param title The new title.
		 * @return This app bar, for chaining.
		 */
		AppBar &setTitle(const std::string &title);

		/**
		 * @brief Choose the elevated look (Paper `elevated`). Restyles at
		 * once.
		 * @param elevated True for the level-2 elevation and its tinted
		 * surface color, false for a flat bar.
		 * @return This app bar, for chaining.
		 */
		AppBar &setElevated(bool elevated);

		/**
		 * @brief Whether the app bar uses the elevated look.
		 * @return True when elevated.
		 */
		bool isElevated(void) const;

		/**
		 * @brief Get the leading navigation button.
		 * @return The button, or nullptr when the app bar has no navigation
		 * icon.
		 */
		std::shared_ptr<IconButton> getNavigationButton(void) const;

		/**
		 * @brief Get a trailing action button.
		 * @param index The position of the action, 0 being the first one
		 * added.
		 * @return The button, or nullptr when `index` is out of range.
		 */
		std::shared_ptr<IconButton> getActionButton(std::size_t index) const;

		/**
		 * @brief Get the title child.
		 * @return The text entity showing the title, or nullptr when no
		 * title child was built (the title has always been empty).
		 */
		std::shared_ptr<Text> getTitle(void) const;

		/**
		 * @brief Padding around the children and spacing between them, in
		 * dp (Paper: 4 dp).
		 */
		static constexpr float HorizontalPadding =
			components::AppBar::HorizontalPadding;

		private:
		/**
		 * @brief Build one standard, medium icon button and register it as
		 * a child of the app bar.
		 * @param action The icon, press handler, accessible name and
		 * disabled flag of the button.
		 * @return The new icon button.
		 */
		std::shared_ptr<IconButton> buildAction(const Action &action);

		/**
		 * @brief Build the navigation, title and action child entities that
		 * are missing and record their identifiers in components::AppBar.
		 *
		 * systems::AppBar decides which children are missing. Does nothing
		 * until the app bar is owned by a shared pointer.
		 */
		void buildContent(void);

		private:
		/**
		 * @brief Construct an app bar whose buttons have no press handler.
		 *
		 * Each glyph name is also used as the accessible name of its
		 * button. The bar is not elevated.
		 * @param registry The registry that stores the components.
		 * @param config The surface configuration (pose, fixed width).
		 * @param variant The variant.
		 * @param title The title text; empty for no title.
		 * @param navigationIcon The glyph name of the navigation icon;
		 * empty for no navigation button.
		 * @param actions The glyph names of the trailing action icons, in
		 * display order.
		 */
		AppBar(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &title,
			   const std::string &navigationIcon,
			   const std::vector<std::string> &actions);

		/**
		 * @brief Construct an app bar with pressable navigation and actions.
		 *
		 * Writes the configuration to components::AppBar and styles the
		 * bar. The children are built by build().
		 * @param registry The registry that stores the components.
		 * @param config The surface configuration (pose, fixed width).
		 * @param variant The variant.
		 * @param title The title text; empty for no title.
		 * @param navigation The leading navigation action; an empty icon
		 * means no navigation button.
		 * @param actions The trailing actions, in display order.
		 * @param elevated Whether the bar uses the elevated look.
		 */
		AppBar(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &title,
			   const Action &navigation, const std::vector<Action> &actions,
			   bool elevated);

		/**
		 * @brief Build the child entities, then style the bar.
		 *
		 * Called once by create(), when the app bar is owned by a shared
		 * pointer and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
