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
#include <memory>
#include <string>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/overlay.hpp"

#include "guillaume/entities/layout.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Menu entity: a transient list of actionable items.
	 *
	 * Menus reuse `SurfaceBase` for the layout, geometry and background surface
	 * and `components::Overlay` for the transient overlay behaviour. Each item
	 * is an `entities::Layout` row holding an optional leading icon and a
	 * label. The Material Design variants are:
	 * - `Dropdown`: an anchored menu opened under a trigger.
	 * - `Context`: a menu opened at the pointer position.
	 * - `Cascading`: a menu whose items open sub-menus.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 * @see entities::Layout
	 */
	class Menu: public SurfaceBase<components::Overlay>
	{
		public:
		/**
		 * @brief Menu variant.
		 */
		enum class Variant { Dropdown, Context, Cascading };

		/**
		 * @brief Describes a single menu item.
		 */
		struct Item {
			std::string label;			///< Item label text.
			std::string leadingIcon;	///< Optional leading icon glyph name.
		};

		/**
		 * @brief Builder used to configure and create `Menu` entities.
		 */
		class Builder: public EntityBuilderBase<Menu>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Dropdown };	   ///< Menu variant.
			std::vector<Item> _items {};			   ///< Menu items.

			public:
			/**
			 * @brief Construct a new Menu Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Menu entity from the current configuration.
			 * @return The newly created Menu.
			 */
			std::shared_ptr<Menu> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the menu pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the menu variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Append a menu item.
			 * @param label The item label.
			 * @param leadingIcon The optional leading icon glyph name.
			 * @return Reference to the builder for chaining.
			 */
			Builder &addItem(const std::string &label,
							 const std::string &leadingIcon = "");
		};

		/**
		 * @brief Director that orchestrates `Menu::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a menu using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The menu variant.
			 * @param items The menu items.
			 * @return The newly created menu.
			 */
			std::shared_ptr<Menu> makeMenu(Builder &builder,
										   std::shared_ptr<ecs::Entity> parent,
										   Variant variant,
										   const std::vector<Item> &items);
		};

		private:
		Variant _variant { Variant::Dropdown };			  ///< Menu variant.
		std::vector<Item> _items {};					  ///< Menu items.
		std::vector<std::shared_ptr<Layout>> _rows {};	  ///< Item rows.
		bool _rowsDirty { true };	 ///< Rows must rebuild.

		private:
		/**
		 * @brief Apply the variant-specific geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Configure the overlay component for the menu.
		 */
		void applyOverlay(void);

		/**
		 * @brief Build (and attach) the item rows for the current items.
		 */
		void buildItems(void);

		/**
		 * @brief Build one item row entity.
		 * @param item The item to build a row for.
		 * @return The newly created row entity.
		 */
		std::shared_ptr<Layout> buildRow(const Item &item);

		public:
		/**
		 * @brief Construct a Menu entity.
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
		 * @brief Set the menu items.
		 * @param items The new items.
		 * @return Reference to this menu for chaining.
		 */
		Menu &setItems(const std::vector<Item> &items);

		/**
		 * @brief Get the number of items.
		 * @return The number of items.
		 */
		std::size_t getItemCount(void) const;

		/**
		 * @brief Get the identifier of an item row entity.
		 * @param index The index of the item.
		 * @return The row entity identifier, or InvalidIdentifier when out of
		 * range.
		 */
		ecs::Entity::Identifier getItemIdentifier(std::size_t index) const;

		/**
		 * @brief Open the menu.
		 * @return Reference to this menu for chaining.
		 */
		Menu &open(void);

		/**
		 * @brief Close the menu.
		 * @return Reference to this menu for chaining.
		 */
		Menu &close(void);

		/**
		 * @brief Whether the menu is open.
		 * @return True when open.
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
		 * @brief Whether the menu is visible.
		 * @return True when visible.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Initialize the menu derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the menu derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
