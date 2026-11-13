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
	 * @brief App bar entity: a surface hosting a leading navigation icon, a
	 * title text and trailing action icons.
	 *
	 * App bars reuse `SurfaceBase` for the layout, geometry and background
	 * surface. They arrange their children along a horizontal axis, centered
	 * on the cross axis. The M3 variants are:
	 * - `Small` and `CenterAligned`: height 64.
	 * - `Medium`: height 112.
	 * - `Large`: height 152.
	 * - `Bottom`: height 80.
	 *
	 * The class is a facade without fields: its state is components::AppBar
	 * and systems::AppBar styles the bar and arranges its children.
	 * `build()` only builds the child entities.
	 *
	 * @see SurfaceBase
	 */
	class AppBar: public SurfaceBase<components::Elevation, components::AppBar>
	{
		public:
		/// App bar variant.
		using Variant = components::AppBar::Variant;

		/// A pressable app bar icon (navigation or action).
		using Action = components::AppBar::Action;

		/**
		 * @brief Builder used to configure and create `AppBar` entities.
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
			 * @brief Construct a new AppBar Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the AppBar entity from the current configuration.
			 * @return The newly created AppBar.
			 */
			std::shared_ptr<AppBar> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the app bar pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the app bar variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the title text.
			 * @param title The new title.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTitle(const std::string &title);

			/**
			 * @brief Set the leading navigation icon glyph name.
			 * @param iconGlyphName The glyph name of the navigation icon.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withNavigationIcon(const std::string &iconGlyphName);

			/**
			 * @brief Append a trailing action icon.
			 * @param iconGlyphName The glyph name of the action icon.
			 * @return Reference to the builder for chaining.
			 */
			Builder &addAction(const std::string &iconGlyphName);

			/**
			 * @brief Append a pressable action (Paper `Appbar.Action`).
			 * @param iconGlyphName Icon glyph.
			 * @param onPress Press handler.
			 * @param accessibilityLabel Accessible name.
			 * @return Reference to this builder for chaining.
			 */
			Builder &addAction(const std::string &iconGlyphName,
							   std::function<void(void)> onPress,
							   const std::string &accessibilityLabel = "");

			/**
			 * @brief Set the navigation press handler (Paper
			 * `Appbar.BackAction`).
			 * @param onPress Press handler.
			 * @param accessibilityLabel Accessible name.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withOnNavigation(
				std::function<void(void)> onPress,
				const std::string &accessibilityLabel = "Navigate up");

			/**
			 * @brief Use the elevated surface (Paper `elevated`).
			 * @param elevated New flag.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withElevated(bool elevated);

			/**
			 * @brief Force the app bar width.
			 * @param width The new width.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withFixedWidth(float width);

			/**
			 * @brief Create an app bar using the builder.
			 * @param parent The parent entity.
			 * @param variant The app bar variant.
			 * @return The newly created app bar.
			 */
			std::shared_ptr<AppBar>
				makeAppBar(std::shared_ptr<ecs::Entity> parent,
						   Variant variant);
		};

		/**
		 * @brief Create the entity, fully built.
		 *
		 * Constructors are private: an entity must be owned before it can
		 * parent its children, so construction and child creation happen
		 * together here.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments.
		 * @return The new entity.
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
		 * @brief Set the app bar variant.
		 * @param variant The new variant.
		 * @return Reference to this app bar for chaining.
		 */
		AppBar &setVariant(Variant variant);

		/**
		 * @brief Get the app bar variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the title text.
		 * @param title The new title.
		 * @return Reference to this app bar for chaining.
		 */
		AppBar &setTitle(const std::string &title);

		/**
		 * @brief Use the elevated surface (Paper `elevated`).
		 * @param elevated New flag.
		 * @return Reference to this app bar for chaining.
		 */
		AppBar &setElevated(bool elevated);

		/**
		 * @brief Check the elevated flag.
		 * @return True when elevated.
		 */
		bool isElevated(void) const;

		/**
		 * @brief Get the navigation icon button.
		 * @return The button, or nullptr.
		 */
		std::shared_ptr<IconButton> getNavigationButton(void) const;

		/**
		 * @brief Get an action icon button.
		 * @param index Action index.
		 * @return The button, or nullptr.
		 */
		std::shared_ptr<IconButton> getActionButton(std::size_t index) const;

		/**
		 * @brief Get the title text.
		 * @return The title, or nullptr.
		 */
		std::shared_ptr<Text> getTitle(void) const;

		/**
		 * @brief Horizontal padding (Paper: 4 dp).
		 */
		static constexpr float HorizontalPadding =
			components::AppBar::HorizontalPadding;

		private:
		/**
		 * @brief Build one pressable icon button.
		 * @param action Action configuration.
		 * @return The icon button child.
		 */
		std::shared_ptr<IconButton> buildAction(const Action &action);

		/**
		 * @brief Build the navigation, title and action child entities that
		 * are missing and record their identifiers in components::AppBar.
		 *
		 * Does nothing until the app bar is owned by a shared pointer.
		 */
		void buildContent(void);

		private:
		/**
		 * @brief Construct an AppBar entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The app bar variant.
		 * @param title The title text.
		 * @param navigationIcon The leading navigation icon glyph name.
		 * @param actions The trailing action icon glyph names.
		 */
		AppBar(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &title,
			   const std::string &navigationIcon,
			   const std::vector<std::string> &actions);

		/**
		 * @brief Construct an app bar with pressable navigation and actions.
		 * @param registry Component registry.
		 * @param config Surface configuration.
		 * @param variant App bar variant.
		 * @param title Title text.
		 * @param navigation Leading navigation action (empty icon: none).
		 * @param actions Trailing actions.
		 * @param elevated Whether the bar uses the elevated surface.
		 */
		AppBar(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &title,
			   const Action &navigation, const std::vector<Action> &actions,
			   bool elevated);

		/**
		 * @brief Create the child entities.
		 *
		 * Called once by create(), when the entity is owned and can be given
		 * as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
