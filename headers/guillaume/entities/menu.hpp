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
	 * @brief Menu entity: a transient list of actionable items (Paper `Menu`
	 * and `Menu.Item`).
	 *
	 * A facade without state: the configuration lives in `components::Menu`,
	 * the visibility in `components::Overlay`, and `systems::Menu` holds
	 * every rule. This class only builds the row entities and forwards its
	 * setters to the component and the system. Each item is a pressable row
	 * holding an optional leading icon, a `bodyLarge` label and an optional
	 * trailing icon. The menu width follows its content within the M3
	 * 112-280 dp range, and the menu can be anchored below or above another
	 * entity. The Material Design variants are:
	 * - `Dropdown`: an anchored menu opened under a trigger.
	 * - `Context`: a menu opened at the pointer position.
	 * - `Cascading`: a menu whose items open sub-menus.
	 *
	 * @see SurfaceBase
	 * @see components::Menu
	 * @see components::Overlay
	 * @see systems::Menu
	 */
	class Menu:
		public SurfaceBase<components::Overlay, components::Elevation,
						   components::Accessibility, components::Menu>
	{
		public:
		/// Menu variant.
		using Variant = components::Menu::Variant;

		/// Surface mode (Paper `mode`).
		using Mode = components::Menu::Mode;

		/// Where the menu opens relative to its anchor.
		using AnchorPosition = components::Menu::AnchorPosition;

		/// Menu item.
		using Item = components::Menu::Item;

		/// Smallest M3 menu width.
		static constexpr float MinWidth = components::Menu::MinWidth;

		/// Largest M3 menu width.
		static constexpr float MaxWidth = components::Menu::MaxWidth;

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

			/**
			 * @brief Build a menu.
			 * @param parent Parent entity.
			 * @param variant Menu variant.
			 * @param items Menu items.
			 * @return The registered menu.
			 */
			std::shared_ptr<Menu> makeMenu(std::shared_ptr<ecs::Entity> parent,
										   Variant variant,
										   const std::vector<Item> &items);
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
		 * @brief Replace the items and rebuild the rows.
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

		private:
		/**
		 * @brief Build one row per item when the rows are stale, record
		 * their identifiers in components::Menu and hand them to the
		 * surface as layout children.
		 *
		 * Does nothing until the menu is owned by a shared pointer.
		 */
		void buildItems(void);

		private:
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
		 * @brief Create the child entities.
		 *
		 * Called once by create(), when the entity is owned and can be given
		 * as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
