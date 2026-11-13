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
#include <utility>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/navigation_bar.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Navigation bar entity: a bottom bar of mutually exclusive
	 * destinations.
	 *
	 * The navigation bar reuses `SurfaceBase` for the row geometry and
	 * `components::SelectionGroup` for the single-selection contract. Each
	 * destination is an internal child entity owning its own layout, a
	 * `components::Selectable`, an icon and a label. The Material Design
	 * variants are:
	 * - `WithLabels`: every destination shows its label.
	 * - `WithoutLabels`: only the selected destination shows its label.
	 *
	 * The class is a facade without fields: its state is
	 * components::NavigationBar and systems::NavigationBar styles the bar
	 * and arranges the destinations. `build()` only builds the
	 * destination child entities.
	 *
	 * @see SurfaceBase
	 * @see components::SelectionGroup
	 */
	class NavigationBar:
		public SurfaceBase<components::SelectionGroup,
						   components::NavigationBar>
	{
		public:
		/// Navigation bar variant.
		using Variant = components::NavigationBar::Variant;

		/// One navigation destination.
		using Destination = components::NavigationBar::Destination;

		/**
		 * @brief Builder used to configure and create `NavigationBar`
		 * entities.
		 */
		class Builder: public EntityBuilderBase<NavigationBar>
		{
			private:
			SurfaceConfig _config;						 ///< Surface config.
			Variant _variant { Variant::WithLabels };	 ///< Bar variant.
			std::vector<std::pair<std::string, std::string>>
				_destinations {};	 ///< Destination (icon, label) pairs.
			std::vector<Destination> _specs {};	   ///< Full destinations.
			std::function<void(std::size_t)>
				_onIndexChange {};	  ///< Index change handler.

			public:
			/**
			 * @brief Construct a new NavigationBar Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the NavigationBar entity from the current
			 * configuration.
			 * @return The newly created NavigationBar.
			 */
			std::shared_ptr<NavigationBar> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the navigation bar pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the navigation bar variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Append a destination.
			 * @param iconGlyph The destination icon glyph name.
			 * @param label The destination label.
			 * @return Reference to the builder for chaining.
			 */
			Builder &addDestination(const std::string &iconGlyph,
									const std::string &label,
									const std::string &focusedIconGlyph = "",
									const std::string &badge			= "");

			/**
			 * @brief Set the handler called with the new index when the user
			 * selects another destination (Paper `onIndexChange`).
			 * @param onIndexChange Index change handler.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withOnIndexChange(
				std::function<void(std::size_t)> onIndexChange);

			/**
			 * @brief Create a navigation bar using the builder.
			 * @param parent The parent entity.
			 * @param variant The navigation bar variant.
			 * @param destinations The destination (icon, label) pairs.
			 * @return The newly created navigation bar.
			 */
			std::shared_ptr<NavigationBar> makeNavigationBar(
				std::shared_ptr<ecs::Entity> parent, Variant variant,
				const std::vector<std::pair<std::string, std::string>>
					&destinations);
		};

		private:
		/**
		 * @brief Build the child entity of every destination that has none
		 * yet, and wire the destinations to the system.
		 */
		void buildDestinations(void);

		public:
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
		static std::shared_ptr<NavigationBar> create(Arguments &&...arguments)
		{
			std::shared_ptr<NavigationBar> entity(
				new NavigationBar(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~NavigationBar(void) override;

		/**
		 * @brief Set the navigation bar variant.
		 * @param variant The new variant.
		 * @return Reference to this bar for chaining.
		 */
		NavigationBar &setVariant(Variant variant);

		/**
		 * @brief Get the navigation bar variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Append a destination.
		 * @param iconGlyph The destination icon glyph name.
		 * @param label The destination label.
		 * @return Reference to this bar for chaining.
		 */
		NavigationBar &addDestination(const std::string &iconGlyph,
									  const std::string &label);

		/**
		 * @brief Append a destination with a focused icon and a badge.
		 * @param destination The destination.
		 * @return Reference to this bar for chaining.
		 */
		NavigationBar &addDestination(const Destination &destination);

		/**
		 * @brief Set the badge of a destination.
		 * @param index Destination index.
		 * @param badge Badge label ("" hides the badge).
		 * @return Reference to this bar for chaining.
		 */
		NavigationBar &setBadge(std::size_t index, const std::string &badge);

		/**
		 * @brief Set the handler called with the new index when the user
		 * selects another destination (Paper `onIndexChange`).
		 * @param onIndexChange Index change handler.
		 * @return Reference to this bar for chaining.
		 */
		NavigationBar &
			setOnIndexChange(std::function<void(std::size_t)> onIndexChange);

		/**
		 * @brief Set the handler called on every destination press, even on
		 * the selected one (Paper `onTabPress`).
		 * @param onTabPress Press handler.
		 * @return Reference to this bar for chaining.
		 */
		NavigationBar &
			setOnTabPress(std::function<void(std::size_t)> onTabPress);

		/**
		 * @brief Get the active-indicator pill of a destination.
		 * @param index Destination index.
		 * @return The pill identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getIndicatorIdentifier(std::size_t index) const;

		/**
		 * @brief Get the icon of a destination.
		 * @param index Destination index.
		 * @return The icon identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getIconIdentifier(std::size_t index) const;

		/**
		 * @brief Get the label of a destination.
		 * @param index Destination index.
		 * @return The label identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getLabelIdentifier(std::size_t index) const;

		/**
		 * @brief Get the badge of a destination.
		 * @param index Destination index.
		 * @return The badge identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getBadgeIdentifier(std::size_t index) const;

		/**
		 * @brief Select a destination by index.
		 * @param index The index of the destination to select.
		 * @return Reference to this bar for chaining.
		 */
		NavigationBar &select(std::size_t index);

		/**
		 * @brief Get the index of the selected destination.
		 * @return The selected destination index.
		 */
		std::size_t getSelectedIndex(void) const;

		/**
		 * @brief Get the number of destinations.
		 * @return The number of destinations.
		 */
		std::size_t getDestinationCount(void) const;

		/**
		 * @brief Get the identifier of a destination child entity.
		 * @param index The index of the destination.
		 * @return The destination entity identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier
			getDestinationIdentifier(std::size_t index) const;

		private:
		/**
		 * @brief Construct a NavigationBar entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The navigation bar variant.
		 * @param destinations The destination (icon, label) pairs.
		 */
		NavigationBar(ecs::ComponentRegistry &registry,
					  const SurfaceConfig &config, Variant variant,
					  const std::vector<std::pair<std::string, std::string>>
						  &destinations);

		/**
		 * @brief Construct a NavigationBar from full destinations.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The navigation bar variant.
		 * @param destinations The destinations.
		 * @param onIndexChange Index change handler.
		 */
		NavigationBar(ecs::ComponentRegistry &registry,
					  const SurfaceConfig &config, Variant variant,
					  const std::vector<Destination> &destinations,
					  std::function<void(std::size_t)> onIndexChange);

		/**
		 * @brief Create the child entities.
		 *
		 * Called once by create(), when the entity is owned and can be given
		 * as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
