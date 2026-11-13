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

#include "guillaume/components/list.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A vertical list of pressable rows, with optional subheaders,
	 * dividers and accordions that fold nested rows (Paper `List.Section`,
	 * `List.Subheader`, `List.Item`, `List.Accordion`, `List.Icon` and
	 * `List.Image`).
	 *
	 * Its state lives in components::List (variant, entries, identifiers
	 * of the rows built for them); systems::List styles the list, sizes
	 * and stacks the rows and shows the children of expanded accordions
	 * only, each frame, in the Layout phase. The class itself holds no
	 * field: it builds the row entities, and its setters write the
	 * component and restyle at once.
	 *
	 * @code
	 * List::Item inbox;
	 * inbox.headline = "Inbox";
	 * inbox.supporting = "3 unread messages";
	 * inbox.leadingIcon = "inbox";
	 * inbox.onPress = [] {
	 * 	// called when the user presses the row
	 * };
	 *
	 * List::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto list = builder.makeList(parent, List::Variant::TwoLine,
	 * 	{ List::subheader("Mail"), inbox, List::divider() });
	 * @endcode
	 *
	 * `List::create(registry, config, variant, items)` builds the same
	 * widget without registering it in an entity registry.
	 *
	 * Things to know:
	 * - A row is an entities::View holding an optional leading image or
	 *   icon, a column with the headline over the supporting text, and an
	 *   optional trailing icon. An entry with children is an accordion: its
	 *   trailing icon is a chevron and a press toggles it before calling
	 *   its press handler.
	 * - The variant only decides whether the supporting text shows:
	 *   `SingleLine` hides it, `TwoLine` and `ThreeLine` show it.
	 * - Indexes are positions among the top-level entries, from 0;
	 *   subheaders and dividers count.
	 * - The width is the fixed width of the surface configuration, in dp
	 *   (components::List::DefaultWidth, 360 dp, with the Builder
	 *   defaults); the row heights follow their content. The list's layout,
	 *   color and corners are overwritten each frame.
	 * - The list is the entity registry that owns its rows. setItems()
	 *   removes every row and builds new ones: identifiers returned before
	 *   by getItemIdentifier(), getChildIdentifier() and
	 *   getContentIdentifier() are then invalid.
	 * - setExpanded() does not call the press handler of the entry.
	 *
	 * @see components::List, systems::List, View
	 */
	class List: public SurfaceBase<components::List>
	{
		public:
		using Variant  = components::List::Variant;		///< List variant.
		using ItemKind = components::List::ItemKind;	///< Kind of an entry.
		using Item	   = components::List::Item;		///< List entry.

		/**
		 * @brief Configures a List, creates it and registers it in an
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<List>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::SingleLine };	 ///< List variant.
			std::vector<Item> _items {};				 ///< List items.

			public:
			/**
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry that stores the
			 * components of the lists built.
			 * @param entityRegistry The registry the lists are added to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a list and its rows from the current
			 * configuration, without registering it or setting its parent.
			 * @return The new list.
			 */
			std::shared_ptr<List> buildEntity(void) override;

			/**
			 * @brief Restore the defaults.
			 *
			 * Defaults: `SingleLine` variant, no entry, default pose,
			 * 360 dp wide (components::List::DefaultWidth).
			 */
			void reset(void) override;

			/**
			 * @brief Set where the list is placed.
			 * @param pose The pose of the list. Default: a
			 * default-constructed pose.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the variant.
			 * @param variant `SingleLine` (the default) hides the
			 * supporting text of the rows; `TwoLine` and `ThreeLine` show
			 * it.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the entries, replacing the ones set before.
			 * @param items The entries, in display order. Default: none.
			 * @return This builder, for chaining.
			 */
			Builder &withItems(const std::vector<Item> &items);

			/**
			 * @brief Set the width of the list and of its rows.
			 * @param width The width, in dp. Default: 360.
			 * @return This builder, for chaining.
			 */
			Builder &withFixedWidth(float width);

			/**
			 * @brief Build a list, register it under `parent` and reset the
			 * builder.
			 * @param parent The entity the list becomes a child of, or
			 * nullptr for a root entity.
			 * @param variant The variant of the list.
			 * @param items The entries, in display order; they replace the
			 * ones given to withItems().
			 * @return The new list.
			 */
			std::shared_ptr<List> makeList(std::shared_ptr<ecs::Entity> parent,
										   Variant variant,
										   const std::vector<Item> &items);
		};

		/**
		 * @brief Make a subheader entry: a section title that is not
		 * pressable.
		 * @param title The text of the subheader. An empty text builds no
		 * row.
		 * @return An entry of kind `ItemKind::Subheader`, to put in the
		 * vector given to the Builder or to setItems().
		 */
		static Item subheader(const std::string &title);

		/**
		 * @brief Make a divider entry: a horizontal line as wide as the
		 * list.
		 * @return An entry of kind `ItemKind::Divider`, to put in the
		 * vector given to the Builder or to setItems().
		 */
		static Item divider(void);

		/**
		 * @brief Create a list and its rows, without registering it in an
		 * entity registry.
		 *
		 * The constructor is private because the rows need the list to be
		 * owned by a shared pointer before they can take it as parent;
		 * create() constructs the list, then builds one row per entry.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments Forwarded to the private constructor:
		 * (registry, config, variant, items).
		 * @return The new list.
		 */
		template<typename... Arguments>
		static std::shared_ptr<List> create(Arguments &&...arguments)
		{
			std::shared_ptr<List> entity(
				new List(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~List(void) override;

		/**
		 * @brief Change the variant. Restyles at once; the rows are kept.
		 * @param variant `SingleLine` hides the supporting text of the
		 * rows; `TwoLine` and `ThreeLine` show it.
		 * @return This list, for chaining.
		 */
		List &setVariant(Variant variant);

		/**
		 * @brief Get the variant.
		 * @return The variant stored in components::List.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Replace the entries.
		 *
		 * Removes every row entity of the list and builds new ones, then
		 * restyles at once. Row identifiers obtained before the call are
		 * invalid afterwards, even for entries that did not change.
		 * @param items The new entries, in display order.
		 * @return This list, for chaining.
		 */
		List &setItems(const std::vector<Item> &items);

		/**
		 * @brief Get the number of top-level entries.
		 * @return How many entries the list has, subheaders and dividers
		 * included, accordion children excluded.
		 */
		std::size_t getItemCount(void) const;

		/**
		 * @brief Get the root entity of a top-level entry: the row
		 * container, the subheader text or the divider.
		 * @param index The index of the top-level entry.
		 * @return The identifier of the entity, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range or
		 * the entry is a subheader without text.
		 */
		ecs::Entity::Identifier getItemIdentifier(std::size_t index) const;

		/**
		 * @brief Get the root entity of a row nested in an accordion.
		 *
		 * The row exists whether the accordion is expanded or not.
		 * @param index The index of the top-level accordion entry.
		 * @param childIndex The position of the row among the children of
		 * that entry, from 0.
		 * @return The identifier of the entity, or
		 * ecs::Entity::InvalidIdentifier when either index is out of range.
		 */
		ecs::Entity::Identifier
			getChildIdentifier(std::size_t index, std::size_t childIndex) const;

		/**
		 * @brief Get the content column of a top-level row: the container
		 * of its headline and supporting text.
		 * @param index The index of the top-level entry.
		 * @return The identifier of the column, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range or
		 * the entry is a subheader or a divider.
		 */
		ecs::Entity::Identifier getContentIdentifier(std::size_t index) const;

		/**
		 * @brief Enable or disable a top-level entry. Restyles at once.
		 *
		 * A disabled row ignores presses.
		 * @param index The index of the top-level entry; nothing happens
		 * when it is out of range.
		 * @param disabled True to disable, false to enable.
		 * @return This list, for chaining.
		 */
		List &setItemDisabled(std::size_t index, bool disabled);

		/**
		 * @brief Expand or collapse a top-level accordion.
		 *
		 * Shows or hides its nested rows and arranges the list again at
		 * once. Does not call the press handler of the entry.
		 * @param index The index of the top-level entry; nothing happens
		 * when it is out of range or when the entry has no children.
		 * @param expanded True to show the nested rows.
		 * @return This list, for chaining.
		 */
		List &setExpanded(std::size_t index, bool expanded);

		/**
		 * @brief Whether a top-level accordion shows its nested rows.
		 * @param index The index of the top-level entry.
		 * @return True when the entry exists and is expanded; false when
		 * `index` is out of range.
		 */
		bool isExpanded(std::size_t index) const;

		private:
		/**
		 * @brief Build one row per entry when the rows are stale, record
		 * their identifiers in components::List, make them pressable and hand
		 * them to the surface as layout children.
		 *
		 * Rows of a previous call are removed first. Does nothing until
		 * the list is owned by a shared pointer.
		 */
		void buildItems(void);

		private:
		/**
		 * @brief Construct a list.
		 *
		 * Writes the configuration to components::List and styles the
		 * list. The rows are built by build().
		 * @param registry The registry that stores the components.
		 * @param config The surface configuration (pose, fixed width).
		 * @param variant The variant.
		 * @param items The entries, in display order.
		 */
		List(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			 Variant variant, const std::vector<Item> &items);

		/**
		 * @brief Build the rows, then style the list.
		 *
		 * Called once by create(), when the list is owned by a shared
		 * pointer and can be given as parent to its rows.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
