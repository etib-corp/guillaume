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

#include "guillaume/components/overlay.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/entities/backdrop.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Material 3 navigation drawer (Paper `Drawer.Section`,
	 * `Drawer.Item` and `Drawer.CollapsedItem`).
	 *
	 * Destinations are pressable rows; the selected one shows the
	 * `secondaryContainer` active pill. Sections add a `titleSmall` header,
	 * dividers separate groups, and the modal variant paints a `backdrop`
	 * behind the sheet. The collapsed variant renders a narrow rail of
	 * icon-over-label destinations.
	 */
	class NavigationDrawer:
		public SurfaceBase<components::Overlay, components::SelectionGroup>
	{
		public:
		/**
		 * @brief Drawer variant.
		 */
		enum class Variant {
			Standard,	 ///< Docked drawer on `surface`.
			Modal,		 ///< Drawer on `surfaceContainerLow` over a backdrop.
			Collapsed	 ///< Narrow rail of collapsed items.
		};

		/**
		 * @brief Kind of a drawer entry.
		 */
		enum class EntryKind {
			Destination,	///< Pressable destination.
			Section,		///< Section header (`titleSmall`).
			Divider			///< Horizontal divider.
		};

		/**
		 * @brief One drawer entry.
		 */
		struct Entry {
			EntryKind kind { EntryKind::Destination };	  ///< Entry kind.
			std::string icon {};		///< Destination icon glyph.
			std::string label {};		///< Destination label or section title.
			std::string right {};		///< Trailing text (badge count, ...).
			bool disabled { false };	///< Whether the destination is
										///< disabled.
		};

		/**
		 * @brief Builder used to configure and create drawers.
		 */
		class Builder: public EntityBuilderBase<NavigationDrawer>
		{
			private:
			SurfaceConfig _config;					   ///< Surface config.
			Variant _variant { Variant::Standard };	   ///< Drawer variant.
			std::vector<Entry> _entries {};			   ///< Drawer entries.
			std::function<void(std::size_t)>
				_onIndexChange {};	  ///< Selection change handler.

			public:
			/**
			 * @brief Construct a drawer builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the drawer.
			 * @return The drawer.
			 */
			std::shared_ptr<NavigationDrawer> buildEntity(void) override;

			/**
			 * @brief Reset the builder to the M3 defaults.
			 */
			void reset(void) override;

			/**
			 * @brief Set the drawer pose.
			 * @param pose The pose.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the drawer variant.
			 * @param variant The variant.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Append a destination.
			 * @param iconGlyph Icon glyph name.
			 * @param label Label text.
			 * @param right Trailing text ("" for none).
			 * @param disabled Whether the destination is disabled.
			 * @return Reference to this builder for chaining.
			 */
			Builder &addDestination(const std::string &iconGlyph,
									const std::string &label,
									const std::string &right = "",
									bool disabled			 = false);

			/**
			 * @brief Append a section header.
			 * @param title Section title.
			 * @return Reference to this builder for chaining.
			 */
			Builder &addSection(const std::string &title);

			/**
			 * @brief Append a divider.
			 * @return Reference to this builder for chaining.
			 */
			Builder &addDivider(void);

			/**
			 * @brief Set the handler called with the new destination index
			 * when the user selects another destination.
			 * @param onIndexChange Handler.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withOnIndexChange(
				std::function<void(std::size_t)> onIndexChange);

			/**
			 * @brief Build a drawer.
			 * @param parent Parent entity.
			 * @param variant Drawer variant.
			 * @param destinations Destination (icon, label) pairs.
			 * @return The registered drawer.
			 */
			std::shared_ptr<NavigationDrawer> makeNavigationDrawer(
				std::shared_ptr<ecs::Entity> parent, Variant variant,
				const std::vector<std::pair<std::string, std::string>>
					&destinations);
		};

		/**
		 * @brief Construct a drawer from (icon, label) pairs.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The drawer variant.
		 * @param destinations The destination (icon, label) pairs.
		 */
		NavigationDrawer(ecs::ComponentRegistry &registry,
						 const SurfaceConfig &config, Variant variant,
						 const std::vector<std::pair<std::string, std::string>>
							 &destinations);

		/**
		 * @brief Construct a drawer from entries.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The drawer variant.
		 * @param entries The drawer entries.
		 * @param onIndexChange Selection change handler.
		 */
		NavigationDrawer(ecs::ComponentRegistry &registry,
						 const SurfaceConfig &config, Variant variant,
						 const std::vector<Entry> &entries,
						 std::function<void(std::size_t)> onIndexChange);

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawer(void) override;

		/**
		 * @brief Set the drawer variant.
		 * @param variant The new variant.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &setVariant(Variant variant);

		/**
		 * @brief Get the drawer variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Select a destination by index (sections and dividers are
		 * not counted).
		 * @param index Destination index.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &select(std::size_t index);

		/**
		 * @brief Get the selected destination index.
		 * @return The selected index.
		 */
		std::size_t getSelectedIndex(void) const;

		/**
		 * @brief Enable or disable a destination.
		 * @param index Destination index.
		 * @param disabled Whether the destination is disabled.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &setDestinationDisabled(std::size_t index,
												 bool disabled);

		/**
		 * @brief Set the selection change handler.
		 * @param onIndexChange Handler receiving the new destination index.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &
			setOnIndexChange(std::function<void(std::size_t)> onIndexChange);

		/**
		 * @brief Show the drawer.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &open(void);

		/**
		 * @brief Hide the drawer.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &close(void);

		/**
		 * @brief Check whether the drawer is shown.
		 * @return True when shown.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Show the drawer.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &show(void);

		/**
		 * @brief Hide the drawer.
		 * @return Reference to this drawer for chaining.
		 */
		NavigationDrawer &hide(void);

		/**
		 * @brief Check whether the drawer is shown.
		 * @return True when shown.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Get the number of destinations.
		 * @return The destination count.
		 */
		std::size_t getDestinationCount(void) const;

		/**
		 * @brief Get the identifier of a destination.
		 * @param index Destination index.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier
			getDestinationIdentifier(std::size_t index) const;

		/**
		 * @brief Get the identifier of the trailing text of a destination.
		 * @param index Destination index.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getRightIdentifier(std::size_t index) const;

		/**
		 * @brief Get the number of section headers.
		 * @return The section count.
		 */
		std::size_t getSectionCount(void) const;

		/**
		 * @brief Get the identifier of a section header.
		 * @param index Section index.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getSectionIdentifier(std::size_t index) const;

		/**
		 * @brief Get the number of dividers.
		 * @return The divider count.
		 */
		std::size_t getDividerCount(void) const;

		/**
		 * @brief Get the identifier of the modal backdrop.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getBackdropIdentifier(void) const;

		/**
		 * @brief Build the entries.
		 */
		void initialize(void) override;

		/**
		 * @brief Restyle and re-arrange the drawer.
		 */
		void update(void) override;

		private:
		Variant _variant { Variant::Standard };	   ///< Drawer variant.
		std::vector<Entry> _entries {};			   ///< Drawer entries.
		std::vector<std::shared_ptr<ecs::Entity>>
			_destinationEntities {};	///< Destination children.
		std::vector<std::shared_ptr<ecs::Entity>>
			_sectionEntities {};	///< Section header children.
		std::vector<std::shared_ptr<ecs::Entity>>
			_dividerEntities {};				///< Divider children.
		std::shared_ptr<Backdrop> _backdrop;	///< Modal backdrop.
		std::function<void(std::size_t)>
			_onIndexChange {};				  ///< Selection change handler.
		bool _updatingSelection { false };	  ///< Reentrancy guard.

		/**
		 * @brief Build the entry children.
		 */
		void buildDestinations(void);

		/**
		 * @brief Select a destination identifier through the selection system.
		 * @param identifier The destination entity identifier to select.
		 */
		void selectEntity(ecs::Entity::Identifier identifier);

		/**
		 * @brief Apply the variant geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Apply the overlay flags and the backdrop.
		 */
		void applyOverlay(void);

		/**
		 * @brief Select the first enabled destination.
		 */
		void applySelection(void);

		/**
		 * @brief Handle a user press on a destination.
		 * @param index Destination index.
		 */
		void pressDestination(std::size_t index);

		/**
		 * @brief Get the drawer width of the current variant.
		 * @return The width.
		 */
		float getDrawerWidth(void) const;
	};

}	 // namespace guillaume::entities
