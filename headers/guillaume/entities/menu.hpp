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
#include <memory>
#include <string>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/elevation.hpp"
#include "guillaume/components/overlay.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Menu entity: a transient list of actionable items (Paper `Menu`
	 * and `Menu.Item`).
	 *
	 * Menus reuse `SurfaceBase` for the layout, geometry and background surface
	 * and `components::Overlay` for the transient overlay behaviour. Each item
	 * is a pressable `entities::View` row holding an optional leading icon, a
	 * `bodyLarge` label and an optional trailing icon. The menu width follows
	 * its content within the M3 112-280 dp range, and the menu can be anchored
	 * below or above another entity. The Material Design variants are:
	 * - `Dropdown`: an anchored menu opened under a trigger.
	 * - `Context`: a menu opened at the pointer position.
	 * - `Cascading`: a menu whose items open sub-menus.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 * @see entities::View
	 */
	class Menu: public SurfaceBase<components::Overlay, components::Elevation>
	{
		public:
		/**
		 * @brief Menu variant.
		 */
		enum class Variant { Dropdown, Context, Cascading };

		/**
		 * @brief Surface mode (Paper `mode`).
		 */
		enum class Mode {
			Elevated,	 ///< Level 2 shadow and tonal tint.
			Flat		 ///< No shadow.
		};

		/**
		 * @brief Where the menu opens relative to its anchor.
		 */
		enum class AnchorPosition {
			Bottom,	   ///< Below the anchor (Paper default).
			Top		   ///< Above the anchor.
		};

		/**
		 * @brief Menu item.
		 */
		struct Item {
			std::string label;			   ///< Item label text.
			std::string leadingIcon;	   ///< Optional leading icon glyph.
			std::string trailingIcon {};   ///< Optional trailing icon glyph.
			std::function<void(void)> onPress {};	 ///< Press handler.
			bool disabled { false };	///< Whether the item is disabled.
			bool dense { false };		///< Whether the item is 32 dp high.
		};

		/**
		 * @brief Smallest M3 menu width.
		 */
		static constexpr float MinWidth = 112.0f;

		/**
		 * @brief Largest M3 menu width.
		 */
		static constexpr float MaxWidth = 280.0f;

		/**
		 * @brief Builder used to configure and create menus.
		 */
		class Builder: public EntityBuilderBase<Menu>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Dropdown };	   ///< Menu variant.
			std::vector<Item> _items {};			   ///< Menu items.

			public:
			/**
			 * @brief Construct a menu builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the menu.
			 * @return The menu.
			 */
			std::shared_ptr<Menu> buildEntity(void) override;

			/**
			 * @brief Reset the builder to the M3 defaults.
			 */
			void reset(void) override;

			/**
			 * @brief Set the menu pose.
			 * @param pose The pose.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the menu variant.
			 * @param variant The variant.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Append an item.
			 * @param label Item label.
			 * @param leadingIcon Optional leading icon glyph.
			 * @return Reference to this builder for chaining.
			 */
			Builder &addItem(const std::string &label,
							 const std::string &leadingIcon = "");

			/**
			 * @brief Append a fully described item.
			 * @param item The item.
			 * @return Reference to this builder for chaining.
			 */
			Builder &addItem(const Item &item);
		};

		/**
		 * @brief Director building menus from item lists.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Build a menu.
			 * @param builder Menu builder.
			 * @param parent Parent entity.
			 * @param variant Menu variant.
			 * @param items Menu items.
			 * @return The registered menu.
			 */
			std::shared_ptr<Menu> makeMenu(Builder &builder,
										   std::shared_ptr<ecs::Entity> parent,
										   Variant variant,
										   const std::vector<Item> &items);
		};

		/**
		 * @brief Construct a menu.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The menu variant.
		 * @param items The menu items.
		 */
		Menu(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			 Variant variant, const std::vector<Item> &items);

		/**
		 * @brief Default destructor.
		 */
		~Menu(void) override;

		/**
		 * @brief Set the menu variant.
		 * @param variant The new variant.
		 * @return Reference to this menu for chaining.
		 */
		Menu &setVariant(Variant variant);

		/**
		 * @brief Get the menu variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the surface mode.
		 * @param mode The new mode.
		 * @return Reference to this menu for chaining.
		 */
		Menu &setMode(Mode mode);

		/**
		 * @brief Get the surface mode.
		 * @return The mode.
		 */
		Mode getMode(void) const;

		/**
		 * @brief Replace the items.
		 * @param items The new items.
		 * @return Reference to this menu for chaining.
		 */
		Menu &setItems(const std::vector<Item> &items);

		/**
		 * @brief Get the number of items.
		 * @return The item count.
		 */
		std::size_t getItemCount(void) const;

		/**
		 * @brief Get the row identifier of an item.
		 * @param index Item index.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getItemIdentifier(std::size_t index) const;

		/**
		 * @brief Enable or disable an item.
		 * @param index Item index.
		 * @param disabled Whether the item is disabled.
		 * @return Reference to this menu for chaining.
		 */
		Menu &setItemDisabled(std::size_t index, bool disabled);

		/**
		 * @brief Anchor the menu to another entity.
		 * @param anchor Anchor entity identifier (InvalidIdentifier clears).
		 * @return Reference to this menu for chaining.
		 */
		Menu &setAnchor(ecs::Entity::Identifier anchor);

		/**
		 * @brief Get the anchor entity.
		 * @return The anchor identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getAnchor(void) const;

		/**
		 * @brief Set where the menu opens relative to its anchor.
		 * @param position The anchor position.
		 * @return Reference to this menu for chaining.
		 */
		Menu &setAnchorPosition(AnchorPosition position);

		/**
		 * @brief Get the anchor position.
		 * @return The anchor position.
		 */
		AnchorPosition getAnchorPosition(void) const;

		/**
		 * @brief Set the handler called when the menu is dismissed (Paper
		 * `onDismiss`); it is also reported by outside clicks and Escape.
		 * @param onDismiss Dismiss handler.
		 * @return Reference to this menu for chaining.
		 */
		Menu &setOnDismiss(std::function<void(void)> onDismiss);

		/**
		 * @brief Hide the menu as a user dismissal and call the dismiss
		 * handler.
		 * @return Reference to this menu for chaining.
		 */
		Menu &dismiss(void);

		/**
		 * @brief Show the menu.
		 * @return Reference to this menu for chaining.
		 */
		Menu &open(void);

		/**
		 * @brief Hide the menu.
		 * @return Reference to this menu for chaining.
		 */
		Menu &close(void);

		/**
		 * @brief Check whether the menu is shown.
		 * @return True when shown.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Show the menu.
		 * @return Reference to this menu for chaining.
		 */
		Menu &show(void);

		/**
		 * @brief Hide the menu.
		 * @return Reference to this menu for chaining.
		 */
		Menu &hide(void);

		/**
		 * @brief Check whether the menu is shown.
		 * @return True when shown.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Build the rows.
		 */
		void initialize(void) override;

		/**
		 * @brief Restyle, size and anchor the menu.
		 */
		void update(void) override;

		private:
		/**
		 * @brief Built row and its content.
		 */
		struct Row {
			std::shared_ptr<View> view;	   ///< Row container.
			std::vector<std::shared_ptr<ecs::Entity>>
				content;					   ///< Row children.
			std::shared_ptr<Text> label;	   ///< Label child.
			std::vector<std::shared_ptr<ecs::Entity>>
				icons;	  ///< Leading/trailing icon children.
		};

		Variant _variant { Variant::Dropdown };	   ///< Menu variant.
		Mode _mode { Mode::Elevated };			   ///< Surface mode.
		std::vector<Item> _items {};			   ///< Menu items.
		std::vector<Row> _rows {};				   ///< Item rows.
		bool _rowsDirty { true };				   ///< Rows must rebuild.
		ecs::Entity::Identifier _anchor {
			ecs::Entity::InvalidIdentifier
		};	  ///< Anchor entity.
		AnchorPosition _anchorPosition {
			AnchorPosition::Bottom
		};	  ///< Anchor position.

		/**
		 * @brief Apply the variant geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Apply the overlay flags.
		 */
		void applyOverlay(void);

		/**
		 * @brief Build or refresh the rows.
		 */
		void buildItems(void);

		/**
		 * @brief Build one row.
		 * @param index Item index.
		 * @return The row.
		 */
		Row buildRow(std::size_t index);

		/**
		 * @brief Restyle one row from its interaction state.
		 * @param index Item index.
		 */
		void styleRow(std::size_t index);

		/**
		 * @brief Size the menu and its rows from the content.
		 */
		void applyWidth(void);

		/**
		 * @brief Move the menu next to its anchor.
		 */
		void applyAnchor(void);
	};

}	 // namespace guillaume::entities
