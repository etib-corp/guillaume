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
#include <utility>
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/overlay.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Navigation drawer entity: a vertical, edge-anchored navigation
	 * surface of mutually exclusive destinations.
	 *
	 * The navigation drawer reuses `SurfaceBase` for the column geometry,
	 * `components::Overlay` for the transient overlay behaviour and
	 * `components::SelectionGroup` for the single-selection contract. Each
	 * destination is an internal child entity owning its own layout, a
	 * `components::Selectable`, an icon and a label. The Material Design
	 * variants are:
	 * - `Standard`: a non-modal drawer docked on the leading edge.
	 * - `Modal`: a modal drawer that captures input.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 * @see components::SelectionGroup
	 */
	class NavigationDrawer:
		public SurfaceBase<components::Overlay, components::SelectionGroup>
	{
		public:
		/**
		 * @brief Navigation drawer variant.
		 */
		enum class Variant { Standard, Modal };

		/**
		 * @brief Builder used to configure and create `NavigationDrawer`
		 * entities.
		 */
		class Builder: public EntityBuilderBase<NavigationDrawer>
		{
			private:
			SurfaceConfig _config;					   ///< Surface config.
			Variant _variant { Variant::Standard };	   ///< Drawer variant.
			std::vector<std::pair<std::string, std::string>>
				_destinations {};	 ///< Destination (icon, label) pairs.

			public:
			/**
			 * @brief Construct a new NavigationDrawer Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the NavigationDrawer entity from the current
			 * configuration.
			 * @return The newly created NavigationDrawer.
			 */
			std::shared_ptr<NavigationDrawer> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the navigation drawer pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the navigation drawer variant.
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
									const std::string &label);
		};

		/**
		 * @brief Director that orchestrates `NavigationDrawer::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a navigation drawer using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The navigation drawer variant.
			 * @param destinations The destination (icon, label) pairs.
			 * @return The newly created navigation drawer.
			 */
			std::shared_ptr<NavigationDrawer> makeNavigationDrawer(
				Builder &builder, std::shared_ptr<ecs::Entity> parent,
				Variant variant,
				const std::vector<std::pair<std::string, std::string>>
					&destinations);
		};

		private:
		Variant _variant { Variant::Standard };	   ///< Drawer variant.
		std::vector<std::pair<std::string, std::string>>
			_destinations {};	 ///< Destination (icon, label) pairs.
		std::vector<std::shared_ptr<ecs::Entity>>
			_destinationEntities {};		  ///< Destination children.
		bool _updatingSelection { false };	  ///< Reentrancy guard.

		private:
		/**
		 * @brief Rebuild the destination children from `_destinations`.
		 */
		void buildDestinations(void);

		/**
		 * @brief Select a destination identifier through the selection system.
		 * @param identifier The destination entity identifier to select.
		 */
		void selectEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Apply the variant-specific geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Configure the overlay component for the current variant.
		 */
		void applyOverlay(void);

		/**
		 * @brief Apply the selection to the children and the group.
		 */
		void applySelection(void);

		public:
		/**
		 * @brief Construct a NavigationDrawer entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The navigation drawer variant.
		 * @param destinations The destination (icon, label) pairs.
		 */
		NavigationDrawer(ecs::ComponentRegistry &registry,
						 const SurfaceConfig &config, Variant variant,
						 const std::vector<std::pair<std::string, std::string>>
							 &destinations);

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawer(void) override;

		/**
		 * @brief Set the navigation drawer variant.
		 * @param variant The new variant.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &setVariant(Variant variant);

		/**
		 * @brief Get the navigation drawer variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Select a destination by index.
		 * @param index The index of the destination to select.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &select(std::size_t index);

		/**
		 * @brief Get the index of the selected destination.
		 * @return The selected destination index.
		 */
		std::size_t getSelectedIndex(void) const;

		/**
		 * @brief Open the navigation drawer.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &open(void);

		/**
		 * @brief Close the navigation drawer.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &close(void);

		/**
		 * @brief Whether the navigation drawer is open.
		 * @return True when open.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Show the entity.
		 * @return Reference to this entity for chaining.
		 */
		NavigationDrawer &show(void);

		/**
		 * @brief Hide the entity.
		 * @return Reference to this entity for chaining.
		 */
		NavigationDrawer &hide(void);

		/**
		 * @brief Whether the entity is visible.
		 * @return True when visible.
		 */
		bool isVisible(void) const;

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

		/**
		 * @brief Initialize the navigation drawer derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the navigation drawer derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
