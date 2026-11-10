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

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"
#include "guillaume/entities/view.hpp"

namespace guillaume::entities
{

	/**
	 * @brief List entity: a vertical container of list items (Paper
	 * `List.Section`, `List.Subheader`, `List.Item`, `List.Accordion`,
	 * `List.Icon` and `List.Image`).
	 *
	 * Each row is a pressable `entities::View` holding an optional leading
	 * slot (icon or image), a content column stacking the `bodyLarge` title
	 * over the `bodyMedium` description, and an optional trailing icon. Row
	 * heights follow the content (16 dp vertical padding), so a one-line row is
	 * 56 dp high as in M3. Subheaders (`titleSmall`, header role) and dividers
	 * structure sections; accordion rows toggle their nested items.
	 *
	 * The variant names the expected number of text lines; it only limits
	 * the description (`SingleLine` hides it).
	 *
	 * @see SurfaceBase
	 * @see entities::View
	 */
	class List: public SurfaceBase<>
	{
		public:
		/**
		 * @brief List variant.
		 */
		enum class Variant { SingleLine, TwoLine, ThreeLine };

		/**
		 * @brief Kind of a list entry.
		 */
		enum class ItemKind {
			Item,		  ///< Pressable row (or accordion when it has
						  ///< children).
			Subheader,	  ///< Section header (`titleSmall`).
			Divider		  ///< Horizontal divider.
		};

		/**
		 * @brief List entry.
		 */
		struct Item {
			std::string headline;		 ///< Primary headline text.
			std::string supporting;		 ///< Optional supporting text.
			std::string leadingIcon;	 ///< Optional leading icon glyph name.
			std::string trailingIcon;	 ///< Optional trailing icon glyph name.
			std::string leadingImage {};	///< Optional leading image path.
			std::function<void(void)> onPress {};	 ///< Press handler.
			bool disabled { false };	///< Whether the row is disabled.
			ItemKind kind { ItemKind::Item };	 ///< Entry kind.
			std::vector<Item> children {};		 ///< Accordion children.
			bool expanded { false };			 ///< Accordion expanded state.
		};

		/**
		 * @brief Builder used to configure and create lists.
		 */
		class Builder: public EntityBuilderBase<List>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::SingleLine };	 ///< List variant.
			std::vector<Item> _items {};				 ///< List items.

			public:
			/**
			 * @brief Construct a list builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the list.
			 * @return The list.
			 */
			std::shared_ptr<List> buildEntity(void) override;

			/**
			 * @brief Reset the builder to the defaults.
			 */
			void reset(void) override;

			/**
			 * @brief Set the list pose.
			 * @param pose The pose.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the list variant.
			 * @param variant The variant.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the list entries.
			 * @param items The entries.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withItems(const std::vector<Item> &items);

			/**
			 * @brief Force the list width.
			 * @param width The width.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withFixedWidth(float width);

			/**
			 * @brief Build a list.
			 * @param parent Parent entity.
			 * @param variant List variant.
			 * @param items List entries.
			 * @return The registered list.
			 */
			std::shared_ptr<List> makeList(std::shared_ptr<ecs::Entity> parent,
										   Variant variant,
										   const std::vector<Item> &items);
		};

		/**
		 * @brief Make a section subheader entry.
		 * @param title Subheader text.
		 * @return The entry.
		 */
		static Item subheader(const std::string &title);

		/**
		 * @brief Make a divider entry.
		 * @return The entry.
		 */
		static Item divider(void);

		/**
		 * @brief Construct a list.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The list variant.
		 * @param items The list entries.
		 */
		List(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			 Variant variant, const std::vector<Item> &items);

		/**
		 * @brief Default destructor.
		 */
		~List(void) override;

		/**
		 * @brief Set the list variant.
		 * @param variant The new variant.
		 * @return Reference to this list for chaining.
		 */
		List &setVariant(Variant variant);

		/**
		 * @brief Get the list variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Replace the entries.
		 * @param items The new entries.
		 * @return Reference to this list for chaining.
		 */
		List &setItems(const std::vector<Item> &items);

		/**
		 * @brief Get the number of top-level entries.
		 * @return The entry count.
		 */
		std::size_t getItemCount(void) const;

		/**
		 * @brief Get the row identifier of a top-level entry.
		 * @param index Entry index.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getItemIdentifier(std::size_t index) const;

		/**
		 * @brief Get the identifier of a nested accordion row.
		 * @param index Accordion entry index.
		 * @param childIndex Child index.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier
			getChildIdentifier(std::size_t index, std::size_t childIndex) const;

		/**
		 * @brief Get the content column of a top-level row.
		 * @param index Entry index.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getContentIdentifier(std::size_t index) const;

		/**
		 * @brief Enable or disable a top-level row.
		 * @param index Entry index.
		 * @param disabled Whether the row is disabled.
		 * @return Reference to this list for chaining.
		 */
		List &setItemDisabled(std::size_t index, bool disabled);

		/**
		 * @brief Expand or collapse an accordion entry.
		 * @param index Entry index.
		 * @param expanded New expanded state.
		 * @return Reference to this list for chaining.
		 */
		List &setExpanded(std::size_t index, bool expanded);

		/**
		 * @brief Check whether an accordion entry is expanded.
		 * @param index Entry index.
		 * @return True when expanded.
		 */
		bool isExpanded(std::size_t index) const;

		/**
		 * @brief Build the rows.
		 */
		void initialize(void) override;

		/**
		 * @brief Restyle and re-arrange the list.
		 */
		void update(void) override;

		private:
		/**
		 * @brief Built row and its content.
		 */
		struct Row {
			std::shared_ptr<ecs::Entity> entity;	///< Row root.
			std::shared_ptr<View> view;				///< Pressable row.
			std::shared_ptr<View> content;		 ///< Title/description column.
			std::shared_ptr<Text> headline;		 ///< Title child.
			std::shared_ptr<Text> supporting;	 ///< Description child.
			std::vector<std::shared_ptr<Icon>> icons;	 ///< Icon children.
			std::shared_ptr<Icon> chevron;				 ///< Accordion chevron.
			std::vector<Row> children;	  ///< Accordion child rows.
			bool hidden { false };		  ///< Collapsed accordion child.
		};

		Variant _variant { Variant::SingleLine };	 ///< List variant.
		std::vector<Item> _items {};				 ///< List entries.
		std::vector<Row> _rows {};					 ///< Built rows.
		bool _rowsDirty { true };	 ///< Whether rows must rebuild.

		/**
		 * @brief Apply the variant geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Build or refresh the rows.
		 */
		void buildItems(void);

		/**
		 * @brief Build one row.
		 * @param item Entry.
		 * @param path Index path used by the press handlers.
		 * @param inset Leading inset for nested rows.
		 * @return The row.
		 */
		Row buildRow(const Item &item, std::vector<std::size_t> path,
					 float inset);

		/**
		 * @brief Build a subheader row: a title text with the Header role.
		 * @param item Entry.
		 * @return The subheader entity, or nullptr when it has no text.
		 */
		std::shared_ptr<ecs::Entity> buildSubheader(const Item &item);

		/**
		 * @brief Build the leading group of a row: the nesting inset, the
		 * leading image or icon, and the headline/supporting column.
		 * @param row Row receiving the created entities.
		 * @param item Entry.
		 * @param inset Leading inset for nested rows.
		 * @return The leading group view.
		 */
		std::shared_ptr<View> buildRowLead(Row &row, const Item &item,
										   float inset);

		/**
		 * @brief Wire the press handler, accessibility and enabled state.
		 * @param row Row to make interactive.
		 * @param item Entry.
		 * @param path Index path used by the press handlers.
		 */
		void makeRowInteractive(Row &row, const Item &item,
								const std::vector<std::size_t> &path);

		/**
		 * @brief Restyle a row from its interaction state.
		 * @param row Row.
		 */
		void styleRow(Row &row);

		/**
		 * @brief Find a row by index path.
		 * @param path Index path.
		 * @return The row, or nullptr.
		 */
		Row *findRow(const std::vector<std::size_t> &path);

		/**
		 * @brief Find an entry by index path.
		 * @param path Index path.
		 * @return The entry, or nullptr.
		 */
		Item *findItem(const std::vector<std::size_t> &path);

		/**
		 * @brief Handle a press on a row.
		 * @param path Index path of the row.
		 */
		void pressRow(const std::vector<std::size_t> &path);

		/**
		 * @brief Get the list width.
		 * @return The width.
		 */
		float getListWidth(void) const;
	};

}	 // namespace guillaume::entities
