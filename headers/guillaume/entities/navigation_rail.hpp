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

#include "guillaume/components/selection.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Navigation rail entity: a vertical bar of mutually exclusive
	 * destinations.
	 *
	 * The navigation rail reuses `SurfaceBase` for the column geometry and
	 * `components::SelectionGroup` for the single-selection contract. Each
	 * destination is an internal child entity owning its own layout, a
	 * `components::Selectable`, an icon and a label. An optional trailing
	 * floating action button can be attached. The Material Design variants are:
	 * - `Collapsed`: width 80, labels hidden.
	 * - `Expanded`: width 256, labels shown.
	 *
	 * @see SurfaceBase
	 * @see components::SelectionGroup
	 */
	class NavigationRail: public SurfaceBase<components::SelectionGroup>
	{
		public:
		/**
		 * @brief Navigation rail variant.
		 */
		enum class Variant { Collapsed, Expanded };

		/**
		 * @brief Builder used to configure and create `NavigationRail`
		 * entities.
		 */
		class Builder: public EntityBuilderBase<NavigationRail>
		{
			private:
			SurfaceConfig _config;						///< Surface config.
			Variant _variant { Variant::Collapsed };	///< Rail variant.
			std::vector<std::pair<std::string, std::string>>
				_destinations {};		 ///< Destination (icon, label) pairs.
			std::string _fabGlyph {};	 ///< Optional FAB glyph name.

			public:
			/**
			 * @brief Construct a new NavigationRail Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the NavigationRail entity from the current
			 * configuration.
			 * @return The newly created NavigationRail.
			 */
			std::shared_ptr<NavigationRail> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the navigation rail pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the navigation rail variant.
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

			/**
			 * @brief Set the optional trailing floating action button glyph.
			 * @param iconGlyph The FAB icon glyph name.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withFab(const std::string &iconGlyph);
		};

		/**
		 * @brief Director that orchestrates `NavigationRail::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a navigation rail using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The navigation rail variant.
			 * @param destinations The destination (icon, label) pairs.
			 * @param fabGlyph The optional FAB glyph name.
			 * @return The newly created navigation rail.
			 */
			std::shared_ptr<NavigationRail> makeNavigationRail(
				Builder &builder, std::shared_ptr<ecs::Entity> parent,
				Variant variant,
				const std::vector<std::pair<std::string, std::string>>
					&destinations,
				const std::string &fabGlyph = "");
		};

		private:
		Variant _variant { Variant::Collapsed };	///< Rail variant.
		std::vector<std::pair<std::string, std::string>>
			_destinations {};		 ///< Destination (icon, label) pairs.
		std::string _fabGlyph {};	 ///< Optional FAB glyph name.
		std::vector<std::shared_ptr<ecs::Entity>>
			_destinationEntities {};		  ///< Destination children.
		std::shared_ptr<Icon> _fabEntity;	  ///< Optional FAB child.
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
		 * @brief Apply the selection to the children and the group.
		 */
		void applySelection(void);

		public:
		/**
		 * @brief Construct a NavigationRail entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The navigation rail variant.
		 * @param destinations The destination (icon, label) pairs.
		 * @param fabGlyph The optional FAB glyph name.
		 */
		NavigationRail(ecs::ComponentRegistry &registry,
					   const SurfaceConfig &config, Variant variant,
					   const std::vector<std::pair<std::string, std::string>>
						   &destinations,
					   const std::string &fabGlyph);

		/**
		 * @brief Default destructor.
		 */
		~NavigationRail(void) override;

		/**
		 * @brief Set the navigation rail variant.
		 * @param variant The new variant.
		 * @return Reference to this rail for chaining.
		 */
		NavigationRail &setVariant(Variant variant);

		/**
		 * @brief Get the navigation rail variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Select a destination by index.
		 * @param index The index of the destination to select.
		 * @return Reference to this rail for chaining.
		 */
		NavigationRail &select(std::size_t index);

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

		/**
		 * @brief Initialize the navigation rail derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the navigation rail derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
