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

#include "guillaume/entities/layout.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief List entity: a vertical container of list items.
	 *
	 * A `List` is a vertical `SurfaceBase` that arranges a stack of row
	 * entities. Each row is an `entities::Layout` holding an optional leading
	 * icon, a headline text, an optional supporting text and an optional
	 * trailing icon. The M3 variants describe the row height:
	 * - `SingleLine`: height 48.
	 * - `TwoLine`: height 64.
	 * - `ThreeLine`: height 88.
	 *
	 * @see SurfaceBase
	 * @see entities::Layout
	 */
	class List: public SurfaceBase<>
	{
		public:
		/**
		 * @brief List item height variant.
		 */
		enum class Variant { SingleLine, TwoLine, ThreeLine };

		/**
		 * @brief Describes a single list item.
		 */
		struct Item {
			std::string headline;		 ///< Primary headline text.
			std::string supporting;		 ///< Optional supporting text.
			std::string leadingIcon;	 ///< Optional leading icon glyph name.
			std::string trailingIcon;	 ///< Optional trailing icon glyph name.
		};

		/**
		 * @brief Builder used to configure and create `List` entities.
		 */
		class Builder: public EntityBuilderBase<List>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::SingleLine };	 ///< List variant.
			std::vector<Item> _items {};				 ///< List items.

			public:
			/**
			 * @brief Construct a new List Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the List entity from the current configuration.
			 * @return The newly created List.
			 */
			std::shared_ptr<List> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the list pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the list variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the list items.
			 * @param items The new items.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withItems(const std::vector<Item> &items);

			/**
			 * @brief Force the list width.
			 * @param width The new fixed width.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withFixedWidth(float width);
		};

		/**
		 * @brief Director that orchestrates `List::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a list using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The list variant.
			 * @param items The list items.
			 * @return The newly created list.
			 */
			std::shared_ptr<List> makeList(Builder &builder,
										   std::shared_ptr<ecs::Entity> parent,
										   Variant variant,
										   const std::vector<Item> &items);
		};

		private:
		Variant _variant { Variant::SingleLine };		  ///< List variant.
		std::vector<Item> _items {};					  ///< List items.
		std::vector<std::shared_ptr<Layout>> _rows {};	  ///< Row entities.
		bool _rowsDirty { true };	 ///< Whether rows must rebuild.

		private:
		/**
		 * @brief Get the row height for the current variant.
		 * @return The row height.
		 */
		float getVariantHeight(void) const;

		/**
		 * @brief Apply the variant-specific surface geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Build (and attach) the row entities for the current items.
		 */
		void buildItems(void);

		/**
		 * @brief Build one row entity from an item.
		 * @param item The item to build a row for.
		 * @return The newly created row entity.
		 */
		std::shared_ptr<Layout> buildRow(const Item &item);

		public:
		/**
		 * @brief Construct a List entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The list variant.
		 * @param items The list items.
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
		 * @brief Set the list items.
		 * @param items The new items.
		 * @return Reference to this list for chaining.
		 */
		List &setItems(const std::vector<Item> &items);

		/**
		 * @brief Get the number of items.
		 * @return The number of items.
		 */
		std::size_t getItemCount(void) const;

		/**
		 * @brief Get the identifier of a row entity.
		 * @param index The index of the item.
		 * @return The row entity identifier, or InvalidIdentifier when out of
		 * range.
		 */
		ecs::Entity::Identifier getItemIdentifier(std::size_t index) const;

		/**
		 * @brief Initialize the list derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the list derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
