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

#include "guillaume/components/selection.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Tabs entity: a horizontal row of mutually exclusive tab labels.
	 *
	 * The tabs reuse `SurfaceBase` for the row geometry and
	 * `components::SelectionGroup` for the single-selection contract. Each tab
	 * is an internal child entity owning its own layout and a
	 * `components::Selectable`.
	 *
	 * The Material Design variants are:
	 * - `Primary`: primary-color labels with a primary active indicator.
	 * - `Secondary`: secondary-color labels with a secondary active indicator.
	 *
	 * @see SurfaceBase
	 * @see components::SelectionGroup
	 */
	class Tabs: public SurfaceBase<components::SelectionGroup>
	{
		public:
		/**
		 * @brief Tabs variant.
		 */
		enum class Variant { Primary, Secondary };

		/**
		 * @brief Builder used to configure and create `Tabs` entities.
		 */
		class Builder: public EntityBuilderBase<Tabs>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Primary };	  ///< Tabs variant.
			std::vector<std::string> _labels {};	  ///< Tab labels.
			std::size_t _selectedIndex { 0 };		  ///< Selected tab.

			public:
			/**
			 * @brief Construct a new Tabs Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Tabs entity from the current configuration.
			 * @return The newly created Tabs.
			 */
			std::shared_ptr<Tabs> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the tabs pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the tabs variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the tab labels.
			 * @param labels The new labels.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabels(const std::vector<std::string> &labels);

			/**
			 * @brief Set the initially selected tab index.
			 * @param selectedIndex The new selected index.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSelectedIndex(std::size_t selectedIndex);
		};

		/**
		 * @brief Director that orchestrates `Tabs::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a tabs entity using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The tabs variant.
			 * @param labels The tab labels.
			 * @param selectedIndex The initially selected tab index.
			 * @return The newly created tabs.
			 */
			std::shared_ptr<Tabs>
				makeTabs(Builder &builder, std::shared_ptr<ecs::Entity> parent,
						 Variant variant,
						 const std::vector<std::string> &labels,
						 std::size_t selectedIndex);
		};

		private:
		Variant _variant { Variant::Primary };	  ///< Tabs variant.
		std::vector<std::string> _labels {};	  ///< Tab labels.
		std::vector<std::shared_ptr<ecs::Entity>>
			_tabs {};						  ///< Tab children.
		std::size_t _selectedIndex { 0 };	  ///< Selected tab index.
		bool _updatingSelection { false };	  ///< Reentrancy guard.

		private:
		/**
		 * @brief Rebuild the tab child entities from `_labels`.
		 */
		void buildTabs(void);

		/**
		 * @brief Select a tab identifier through the selection system.
		 * @param identifier The tab entity identifier to select.
		 */
		void selectEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Apply the variant-specific label and indicator colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Apply the selection to the tab children and group.
		 */
		void applySelection(void);

		public:
		/**
		 * @brief Construct a Tabs entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The tabs variant.
		 * @param labels The tab labels.
		 * @param selectedIndex The initially selected tab index.
		 */
		Tabs(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			 Variant variant, const std::vector<std::string> &labels,
			 std::size_t selectedIndex);

		/**
		 * @brief Default destructor.
		 */
		~Tabs(void) override;

		/**
		 * @brief Set the tabs variant.
		 * @param variant The new variant.
		 * @return Reference to this tabs for chaining.
		 */
		Tabs &setVariant(Variant variant);

		/**
		 * @brief Get the tabs variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the tab labels.
		 * @param labels The new labels.
		 * @return Reference to this tabs for chaining.
		 */
		Tabs &setLabels(const std::vector<std::string> &labels);

		/**
		 * @brief Select a tab by index.
		 * @param index The index of the tab to select.
		 * @return Reference to this tabs for chaining.
		 */
		Tabs &select(std::size_t index);

		/**
		 * @brief Get the index of the selected tab.
		 * @return The selected tab index.
		 */
		std::size_t getSelectedIndex(void) const;

		/**
		 * @brief Get the number of tabs.
		 * @return The number of tabs.
		 */
		std::size_t getTabCount(void) const;

		/**
		 * @brief Get the identifier of a tab child entity.
		 * @param index The index of the tab.
		 * @return The tab entity identifier, or InvalidIdentifier when out of
		 * range.
		 */
		ecs::Entity::Identifier getTabIdentifier(std::size_t index) const;

		/**
		 * @brief Initialize the tabs derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the tabs derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
