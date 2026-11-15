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
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/menu.hpp"
#include "guillaume/components/overlay.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A pop-up column of pressable items, shown on demand next to an
	 * anchor entity (Paper `Menu` and `Menu.Item`).
	 *
	 * Its state lives in components::Menu (variant, mode, items, row
	 * identifiers, anchor); whether it is shown and its dismiss handler
	 * live in components::Overlay. systems::Menu styles the menu, sizes
	 * its rows and moves it next to its anchor each frame, in the Layout
	 * phase. The class itself holds no field: it builds the row entities,
	 * and its setters write the component and restyle at once.
	 *
	 * @code
	 * Menu::Item remove;
	 * remove.label = "Delete";
	 * remove.leadingIcon = "delete";
	 * remove.onPress = [] {
	 * 	// called when the user presses the row
	 * };
	 *
	 * Menu::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto menu = builder.addItem("Rename", "edit")
	 * 	.addItem(remove)
	 * 	.makeMenu(parent, Menu::Variant::Dropdown, {});
	 * menu->setAnchor(button->getIdentifier());
	 * menu->open();
	 * @endcode
	 *
	 * `Menu::create(registry, config, variant, items)` builds the same
	 * widget without registering it in an entity registry.
	 *
	 * Things to know:
	 * - The menu is closed at creation: call open().
	 * - Pressing an item calls its press handler and leaves the menu open;
	 *   close it from the handler when needed.
	 * - close() and hide() do not call the dismiss handler themselves;
	 *   dismiss() and Escape do.
	 * - Each row holds an optional leading icon, a label and an optional
	 *   trailing icon. A row is 48 dp high, or 32 dp when the item is
	 *   dense. The width follows the widest row, kept between MinWidth
	 *   (112 dp) and MaxWidth (280 dp); it is overwritten each frame, like
	 *   the menu's layout, color, corners, elevation and overlay flags.
	 * - With an anchor, the pose is overwritten each frame: the menu takes
	 *   the position of the anchor and sits 4 dp below or above it.
	 *   Without one, the menu stays at the pose it was given.
	 * - The variant is stored but systems::Menu does not read it: the
	 *   three variants look and behave the same.
	 * - The menu is the entity registry that owns its rows. setItems()
	 *   removes every row and builds new ones: identifiers returned before
	 *   by getItemIdentifier() are then invalid.
	 *
	 * @see components::Menu, systems::Menu, components::Overlay
	 */
	class Menu:
		public SurfaceBase<components::Overlay, components::Elevation,
						   components::Accessibility, components::Menu>
	{
		public:
		/// Menu variant (`Dropdown`, `Context`, `Cascading`); stored only.
		using Variant = components::Menu::Variant;

		/// Surface mode (Paper `mode`): `Elevated` (level-2 shadow and
		/// tint) or `Flat` (no shadow).
		using Mode = components::Menu::Mode;

		/// Where the menu opens relative to its anchor: `Bottom` (below)
		/// or `Top` (above).
		using AnchorPosition = components::Menu::AnchorPosition;

		/// Menu item: label, leading and trailing icon glyph names, press
		/// handler, disabled and dense flags.
		using Item = components::Menu::Item;

		/// Smallest M3 menu width, in dp.
		static constexpr float MinWidth = components::Menu::MinWidth;

		/// Largest M3 menu width, in dp.
		static constexpr float MaxWidth = components::Menu::MaxWidth;

		/**
		 * @brief Configures a Menu, creates it and registers it in an
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<Menu>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Dropdown };	   ///< Menu variant.
			std::vector<Item> _items {};			   ///< Menu items.

			public:
			/**
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry that stores the
			 * components of the menus built.
			 * @param entityRegistry The registry the menus are added to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a menu and its rows from the current
			 * configuration, without registering it or setting its parent.
			 * @return The new menu.
			 */
			std::shared_ptr<Menu> buildEntity(void) override;

			/**
			 * @brief Restore the defaults.
			 *
			 * Defaults: `Dropdown` variant, no item, default pose. The
			 * mode (`Elevated`), the anchor (none) and the anchor position
			 * (`Bottom`) are set on the menu after it is built.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the menu is placed while it has no anchor.
			 * @param pose The pose of the menu. Default: a
			 * default-constructed pose.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the variant.
			 * @param variant The variant to store. Default: `Dropdown`.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Append an enabled item without press handler.
			 * @param label The text of the item.
			 * @param leadingIcon The glyph name of the icon shown before
			 * the label; empty for none.
			 * @return This builder, for chaining.
			 */
			Builder &addItem(const std::string &label,
							 const std::string &leadingIcon = "");

			/**
			 * @brief Append an item described in full.
			 * @param item The label, icons, press handler and flags of the
			 * item.
			 * @return This builder, for chaining.
			 */
			Builder &addItem(const Item &item);

			/**
			 * @brief Build a menu, register it under `parent` and reset the
			 * builder.
			 *
			 * The items given here come after the ones already added with
			 * addItem().
			 * @param parent The entity the menu becomes a child of, or
			 * nullptr for a root entity.
			 * @param variant The variant of the menu.
			 * @param items The items to append, in display order; may be
			 * empty.
			 * @return The new menu, closed.
			 */
			std::shared_ptr<Menu> makeMenu(std::shared_ptr<ecs::Entity> parent,
										   Variant variant,
										   const std::vector<Item> &items);
		};

		/**
		 * @brief Create a menu and its rows, without registering it in an
		 * entity registry.
		 *
		 * The constructor is private because the rows need the menu to be
		 * owned by a shared pointer before they can take it as parent;
		 * create() constructs the menu, then builds one row per item.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments Forwarded to the private constructor:
		 * (registry, config, variant, items).
		 * @return The new menu, closed.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Menu> create(Arguments &&...arguments)
		{
			std::shared_ptr<Menu> entity(
				new Menu(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~Menu(void) override = default;

		/**
		 * @brief Change the variant stored in components::Menu.
		 *
		 * Runs systems::Menu::apply at once, which does not read the
		 * variant: the look and behaviour of the menu do not change.
		 * @param variant The new variant.
		 * @return This menu, for chaining.
		 */
		Menu &setVariant(Variant variant);

		/**
		 * @brief Get the variant.
		 * @return The variant stored in components::Menu.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Change the surface mode (Paper `mode`). Restyles at once.
		 * @param mode `Elevated` (the default) for the level-2 elevation
		 * and its tinted surface color, `Flat` for no shadow.
		 * @return This menu, for chaining.
		 */
		Menu &setMode(Mode mode);

		/**
		 * @brief Get the surface mode.
		 * @return `Elevated` or `Flat`.
		 */
		Mode getMode(void) const;

		/**
		 * @brief Replace the items.
		 *
		 * Removes every row entity of the menu and builds new ones, then
		 * restyles at once. Row identifiers obtained before the call are
		 * invalid afterwards, even for items that did not change.
		 * @param items The new items, in display order.
		 * @return This menu, for chaining.
		 */
		Menu &setItems(const std::vector<Item> &items);

		/**
		 * @brief Get the number of items.
		 * @return How many items the menu has.
		 */
		std::size_t getItemCount(void) const;

		/**
		 * @brief Get the row entity of an item: the pressable container of
		 * its icons and label.
		 * @param index The index of the item, from 0.
		 * @return The identifier of the row, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range.
		 */
		ecs::Entity::Identifier getItemIdentifier(std::size_t index) const;

		/**
		 * @brief Enable or disable an item. Restyles at once.
		 *
		 * A disabled item ignores presses.
		 * @param index The index of the item; nothing happens when it is
		 * out of range.
		 * @param disabled True to disable, false to enable.
		 * @return This menu, for chaining.
		 */
		Menu &setItemDisabled(std::size_t index, bool disabled);

		/**
		 * @brief Anchor the menu to another entity.
		 *
		 * While the anchor has a Transform and a Bound, systems::Menu
		 * moves the menu to the anchor's position, 4 dp below or above it
		 * (see setAnchorPosition()), at once and then each frame.
		 * @param anchor The identifier of the anchor entity, or
		 * ecs::Entity::InvalidIdentifier to stop following an anchor; the
		 * menu then keeps its current pose.
		 * @return This menu, for chaining.
		 */
		Menu &setAnchor(ecs::Entity::Identifier anchor);

		/**
		 * @brief Get the anchor entity.
		 * @return The identifier of the anchor, or
		 * ecs::Entity::InvalidIdentifier when the menu has none.
		 */
		ecs::Entity::Identifier getAnchor(void) const;

		/**
		 * @brief Choose on which side of its anchor the menu opens. Moves
		 * the menu at once.
		 * @param position `Bottom` (the default) to open below the anchor,
		 * `Top` to open above it.
		 * @return This menu, for chaining.
		 */
		Menu &setAnchorPosition(AnchorPosition position);

		/**
		 * @brief Get the side of the anchor the menu opens on.
		 * @return `Bottom` or `Top`.
		 */
		AnchorPosition getAnchorPosition(void) const;

		/**
		 * @brief Set the handler called when the menu is dismissed (Paper
		 * `onDismiss`).
		 *
		 * The handler is stored in the menu's components::Overlay.
		 * @param onDismiss Called after dismiss() or Escape closed the
		 * menu. close() and hide() do not call it themselves.
		 * @return This menu, for chaining.
		 */
		Menu &setOnDismiss(std::function<void(void)> onDismiss);

		/**
		 * @brief Close the menu as the user would: hide it, then call the
		 * dismiss handler.
		 *
		 * Does nothing when the menu is already closed.
		 * @return This menu, for chaining.
		 */
		Menu &dismiss(void);

		/**
		 * @brief Open the menu. Same as show().
		 * @return This menu, for chaining.
		 */
		Menu &open(void);

		/**
		 * @brief Close the menu without calling the dismiss handler. Same
		 * as hide().
		 * @return This menu, for chaining.
		 */
		Menu &close(void);

		/**
		 * @brief Whether the menu is open. Same as isVisible().
		 * @return True when shown; false at creation.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Move the menu next to its anchor, then show it.
		 * @return This menu, for chaining.
		 */
		Menu &show(void);

		/**
		 * @brief Hide the menu without calling the dismiss handler.
		 * @return This menu, for chaining.
		 */
		Menu &hide(void);

		/**
		 * @brief Whether the menu is shown.
		 * @return True when its components::Overlay is visible; false at
		 * creation.
		 */
		bool isVisible(void) const;

		private:
		/**
		 * @brief Build one row per item when the rows are stale, record
		 * their identifiers in components::Menu and hand them to the
		 * surface as layout children.
		 *
		 * Rows of a previous call are removed first. Does nothing until
		 * the menu is owned by a shared pointer.
		 */
		void buildItems(void);

		private:
		/**
		 * @brief Construct a menu.
		 *
		 * Writes the configuration to components::Menu and styles the
		 * menu. The rows are built by build().
		 * @param registry The registry that stores the components.
		 * @param config The surface configuration (pose).
		 * @param variant The variant to store.
		 * @param items The items, in display order.
		 */
		Menu(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			 Variant variant, const std::vector<Item> &items);

		/**
		 * @brief Build the rows, then style the menu.
		 *
		 * Called once by create(), when the menu is owned by a shared
		 * pointer and can be given as parent to its rows.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
