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
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/drag_interaction.hpp"
#include "guillaume/components/scrollable.hpp"
#include "guillaume/components/value.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Carousel entity: a horizontally scrollable strip of items.
	 *
	 * The carousel reuses `SurfaceBase` for its geometry, `components::Value`
	 * to track the current item index, `components::Scrollable` for the
	 * content offset and `components::DragInteraction` (driven by
	 * `systems::Drag`) for navigation. Each item is a fixed-size child laid
	 * out along the horizontal axis. The variants are:
	 * - `Basic`: items laid out with a small padding.
	 * - `Centered`: items centered on the cross axis.
	 * - `Uncontained`: items laid out without container padding.
	 *
	 * @see SurfaceBase
	 * @see components::Value
	 * @see components::Scrollable
	 * @see components::DragInteraction
	 */
	class Carousel:
		public SurfaceBase<components::Value, components::Scrollable,
						   components::DragInteraction>
	{
		public:
		/**
		 * @brief Carousel visual variant.
		 */
		enum class Variant {
			Basic,		   ///< Padded item strip.
			Centered,	   ///< Centered item strip.
			Uncontained	   ///< Unpadded, overflowing item strip.
		};

		/**
		 * @brief Builder used to configure and create `Carousel` entities.
		 */
		class Builder: public EntityBuilderBase<Carousel>
		{
			private:
			SurfaceConfig _config;					///< Surface configuration.
			Variant _variant { Variant::Basic };	///< Visual variant.
			std::size_t _itemCount { 3 };			///< Number of items.
			float _itemWidth { 120.0f };			///< Item width.
			float _itemHeight { 160.0f };			///< Item height.
			std::size_t _value { 0 };				///< Initial item index.
			std::vector<std::shared_ptr<ecs::Entity>>
				_items {};	  ///< Custom item entities.
			std::function<void(std::size_t)> _onChanged {};	   ///< Callback.

			public:
			/**
			 * @brief Construct a new Carousel Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Carousel entity from the current configuration.
			 * @return The newly created Carousel.
			 */
			std::shared_ptr<Carousel> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the carousel pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the carousel variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the number of placeholder items.
			 * @param itemCount The new item count.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withItemCount(std::size_t itemCount);

			/**
			 * @brief Set the item size.
			 * @param width The item width.
			 * @param height The item height.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withItemSize(float width, float height);

			/**
			 * @brief Set the current item index.
			 * @param index The new item index.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withValue(std::size_t index);

			/**
			 * @brief Set the index change callback.
			 * @param onChanged The callback invoked when the index changes.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withOnChanged(
				const std::function<void(std::size_t)> &onChanged);

			/**
			 * @brief Supply custom item entities.
			 * @param items The item entities to attach.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withItems(
				const std::vector<std::shared_ptr<ecs::Entity>> &items);
		};

		/**
		 * @brief Director that orchestrates `Carousel::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a carousel using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The carousel variant.
			 * @param itemCount The number of placeholder items.
			 * @return The newly created carousel.
			 */
			std::shared_ptr<Carousel>
				makeCarousel(Builder &builder,
							 std::shared_ptr<ecs::Entity> parent,
							 Variant variant, std::size_t itemCount);
		};

		private:
		Variant _variant { Variant::Basic };	///< Visual variant.
		std::size_t _itemCount { 0 };			///< Number of items.
		float _itemWidth { 120.0f };			///< Item width.
		float _itemHeight { 160.0f };			///< Item height.
		std::vector<std::shared_ptr<ecs::Entity>>
			_items {};									   ///< Item entities.
		std::function<void(std::size_t)> _onChanged {};	   ///< User callback.
		bool _built { false };	  ///< Whether items exist.

		private:
		/**
		 * @brief Build (and attach) the item entities.
		 */
		void buildItems(void);

		/**
		 * @brief Apply the variant padding and alignment.
		 */
		void applyVariant(void);

		/**
		 * @brief Refresh the scroll offset from the current index.
		 */
		void applyState(void);

		/**
		 * @brief Commit an index, firing the user callback when it changes.
		 * @param index The new index.
		 */
		void setIndex(std::size_t index);

		public:
		/**
		 * @brief Construct a Carousel entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The visual variant.
		 * @param itemCount The number of placeholder items.
		 * @param itemWidth The item width.
		 * @param itemHeight The item height.
		 * @param value The initial item index.
		 * @param items The custom item entities, used when non-empty.
		 * @param onChanged The index change callback.
		 */
		Carousel(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				 Variant variant, std::size_t itemCount, float itemWidth,
				 float itemHeight, std::size_t value,
				 const std::vector<std::shared_ptr<ecs::Entity>> &items,
				 const std::function<void(std::size_t)> &onChanged);

		/**
		 * @brief Default destructor.
		 */
		~Carousel(void) override;

		/**
		 * @brief Set the carousel variant.
		 * @param variant The new variant.
		 * @return Reference to this carousel for chaining.
		 */
		Carousel &setVariant(Variant variant);

		/**
		 * @brief Get the carousel variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the number of placeholder items.
		 * @param itemCount The new item count.
		 * @return Reference to this carousel for chaining.
		 */
		Carousel &setItemCount(std::size_t itemCount);

		/**
		 * @brief Get the number of items.
		 * @return The item count.
		 */
		std::size_t getItemCount(void) const;

		/**
		 * @brief Get the identifier of an item entity.
		 * @param index The item index.
		 * @return The item identifier, or InvalidIdentifier when out of range.
		 */
		ecs::Entity::Identifier getItemIdentifier(std::size_t index) const;

		/**
		 * @brief Set the current item index.
		 * @param index The new index.
		 * @return Reference to this carousel for chaining.
		 */
		Carousel &setValue(std::size_t index);

		/**
		 * @brief Get the current item index.
		 * @return The current index.
		 */
		std::size_t getValue(void) const;

		/**
		 * @brief Initialize the carousel derived state and items.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the carousel derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
