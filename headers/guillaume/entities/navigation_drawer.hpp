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

#include "guillaume/components/navigation_drawer.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/entities/backdrop.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A side sheet listing destinations, of which exactly one is
	 * selected, with optional section titles and dividers (Paper
	 * `Drawer.Section`, `Drawer.Item` and `Drawer.CollapsedItem`).
	 *
	 * Its state lives in components::NavigationDrawer (variant, entries,
	 * child identifiers, index-change handler); systems::NavigationDrawer
	 * sizes and colors the sheet, places the entries in a column and
	 * drives the backdrop each frame, in the Layout phase. Which
	 * destination is selected is stored in the components::Selectable of
	 * each destination child; whether the drawer is shown is stored in its
	 * components::Overlay. The class itself holds no field.
	 *
	 * @code
	 * NavigationDrawer::Builder builder(getComponentRegistry(),
	 * 	getEntityRegistry());
	 * auto drawer = builder.addSection("Mail")
	 * 	.addDestination("inbox", "Inbox", "24")
	 * 	.addDestination("send", "Sent")
	 * 	.addDivider()
	 * 	.withOnIndexChange([](std::size_t index) {
	 * 		// called when the user selects another destination
	 * 	})
	 * 	.makeNavigationDrawer(parent, NavigationDrawer::Variant::Modal, {});
	 * drawer->open();
	 * @endcode
	 *
	 * `NavigationDrawer::create(registry, config, variant, entries,
	 * onIndexChange)` builds the same widget without registering it in an
	 * entity registry.
	 *
	 * Things to know:
	 * - A destination index counts destinations only, from 0: sections and
	 *   dividers are skipped.
	 * - The drawer is closed at creation: isOpen() is false until open().
	 * - The first enabled destination is selected at creation. select()
	 *   does not call the index-change handler and does not check the
	 *   disabled flag; only a press by the user on another, enabled
	 *   destination calls the handler.
	 * - The size is fixed by the variant and overwritten each frame:
	 *   320 dp wide (`Standard`, `Modal`) or 80 dp wide (`Collapsed`),
	 *   640 dp high. The color, corners, padding and spacing are
	 *   overwritten too.
	 * - `Modal` dims what is behind the drawer with a backdrop while it is
	 *   open, and sets its components::Overlay to be dismissed by a click
	 *   outside. `Collapsed` is a narrow rail with the label under the
	 *   icon, without section titles or trailing texts.
	 * - The drawer is the entity registry that owns its children.
	 *   setVariant() removes and rebuilds every entry child when it
	 *   switches between `Collapsed` and another variant: the identifiers
	 *   returned before by getDestinationIdentifier(), getRightIdentifier()
	 *   and getSectionIdentifier() are then invalid. The backdrop is kept.
	 *
	 * @see components::NavigationDrawer, systems::NavigationDrawer,
	 * NavigationBar
	 */
	class NavigationDrawer:
		public SurfaceBase<components::Overlay, components::SelectionGroup,
						   components::NavigationDrawer>
	{
		public:
		/// Variant: `Standard` (docked sheet), `Modal` (sheet over a
		/// backdrop) or `Collapsed` (narrow rail).
		using Variant = components::NavigationDrawer::Variant;

		/// Kind of a drawer entry: destination, section title or divider.
		using EntryKind = components::NavigationDrawer::EntryKind;

		/// One drawer entry: kind, icon glyph name, label, trailing text
		/// and disabled flag.
		using Entry = components::NavigationDrawer::Entry;

		/**
		 * @brief Configures a NavigationDrawer, creates it and registers it
		 * in an entity registry.
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
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry that stores the
			 * components of the drawers built.
			 * @param entityRegistry The registry the drawers are added to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a drawer and its children from the current
			 * configuration, without registering it or setting its parent.
			 * @return The new drawer.
			 */
			std::shared_ptr<NavigationDrawer> buildEntity(void) override;

			/**
			 * @brief Restore the defaults.
			 *
			 * Defaults: `Standard` variant, no entry, no index-change
			 * handler, default pose, 320 dp wide and 640 dp high
			 * (components::NavigationDrawer::Width and Height), 12 dp of
			 * padding and 4 dp between entries.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the drawer is placed.
			 * @param pose The pose of the drawer. Default: a
			 * default-constructed pose.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the variant.
			 * @param variant `Standard` (the default), `Modal` or
			 * `Collapsed`.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Append a destination after the entries already added.
			 * @param iconGlyph The glyph name of the icon, such as
			 * `"inbox"`; empty for no icon.
			 * @param label The text of the destination.
			 * @param right A short text shown at the far edge of the row,
			 * such as a count; empty for none. Not shown by the `Collapsed`
			 * variant.
			 * @param disabled True to make the destination ignore presses.
			 * @return This builder, for chaining.
			 */
			Builder &addDestination(const std::string &iconGlyph,
									const std::string &label,
									const std::string &right = "",
									bool disabled			 = false);

			/**
			 * @brief Append a section title after the entries already
			 * added.
			 * @param title The title text. The `Collapsed` variant builds
			 * no child for it.
			 * @return This builder, for chaining.
			 */
			Builder &addSection(const std::string &title);

			/**
			 * @brief Append a horizontal divider after the entries already
			 * added.
			 * @return This builder, for chaining.
			 */
			Builder &addDivider(void);

			/**
			 * @brief Set the handler called when the user selects another
			 * destination.
			 * @param onIndexChange Called with the index of the newly
			 * selected destination (sections and dividers are not counted).
			 * Not called by select(). Default: none.
			 * @return This builder, for chaining.
			 */
			Builder &withOnIndexChange(
				std::function<void(std::size_t)> onIndexChange);

			/**
			 * @brief Build a drawer, register it under `parent` and reset
			 * the builder.
			 *
			 * The destinations given here come after the entries already
			 * added with addDestination(), addSection() and addDivider().
			 * @param parent The entity the drawer becomes a child of, or
			 * nullptr for a root entity.
			 * @param variant The variant of the drawer.
			 * @param destinations The (icon glyph name, label) pair of each
			 * destination to append; may be empty.
			 * @return The new drawer.
			 */
			std::shared_ptr<NavigationDrawer> makeNavigationDrawer(
				std::shared_ptr<ecs::Entity> parent, Variant variant,
				const std::vector<std::pair<std::string, std::string>>
					&destinations);
		};

		/**
		 * @brief Create a drawer and its children, without registering it
		 * in an entity registry.
		 *
		 * The constructors are private because the children need the
		 * drawer to be owned by a shared pointer before they can take it as
		 * parent; create() constructs the drawer, builds the backdrop and
		 * one child per entry, and selects the first enabled destination.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments Forwarded to a private constructor: either
		 * (registry, config, variant, entries, onIndexChange) with `Entry`
		 * values, or (registry, config, variant, destinations) with (icon
		 * glyph name, label) pairs.
		 * @return The new drawer.
		 */
		template<typename... Arguments> static std::shared_ptr<NavigationDrawer>
			create(Arguments &&...arguments)
		{
			std::shared_ptr<NavigationDrawer> entity(
				new NavigationDrawer(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~NavigationDrawer(void) override;

		/**
		 * @brief Change the variant.
		 *
		 * Restyles at once. Switching between `Collapsed` and another
		 * variant removes every entry child and builds new ones (a rail
		 * and a sheet are made of different children), then selects the
		 * same destination index again; identifiers of entry children
		 * obtained before are then invalid. This rebuild is skipped when
		 * the drawer has no destination child. Switching between
		 * `Standard` and `Modal` keeps the children.
		 * @param variant The new variant.
		 * @return This drawer, for chaining.
		 */
		NavigationDrawer &setVariant(Variant variant);

		/**
		 * @brief Get the variant.
		 * @return The variant stored in components::NavigationDrawer.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Select a destination and deselect the others.
		 *
		 * Restyles at once. Does not call the index-change handler and
		 * does not check whether the destination is disabled.
		 * @param index The index of the destination, sections and dividers
		 * not counted; nothing happens when it is out of range.
		 * @return This drawer, for chaining.
		 */
		NavigationDrawer &select(std::size_t index);

		/**
		 * @brief Get the index of the selected destination.
		 * @return The index, sections and dividers not counted, or 0 when
		 * no destination is selected.
		 */
		std::size_t getSelectedIndex(void) const;

		/**
		 * @brief Enable or disable a destination. Restyles it at once.
		 *
		 * A disabled destination ignores presses. Disabling the selected
		 * destination does not move the selection.
		 * @param index The index of the destination, sections and dividers
		 * not counted; nothing happens when it is out of range.
		 * @param disabled True to disable, false to enable.
		 * @return This drawer, for chaining.
		 */
		NavigationDrawer &setDestinationDisabled(std::size_t index,
												 bool disabled);

		/**
		 * @brief Set the handler called when the user selects another
		 * destination.
		 * @param onIndexChange Called with the index of the newly selected
		 * destination, after the selection changed. Not called by select()
		 * nor when the user presses the selected destination.
		 * @return This drawer, for chaining.
		 */
		NavigationDrawer &
			setOnIndexChange(std::function<void(std::size_t)> onIndexChange);

		/**
		 * @brief Open the drawer. Same as show().
		 * @return This drawer, for chaining.
		 */
		NavigationDrawer &open(void);

		/**
		 * @brief Close the drawer. Same as hide().
		 * @return This drawer, for chaining.
		 */
		NavigationDrawer &close(void);

		/**
		 * @brief Whether the drawer is open. Same as isVisible().
		 * @return True between open() and close(); false at creation.
		 */
		bool isOpen(void) const;

		/**
		 * @brief Show the drawer, and its backdrop when the variant is
		 * `Modal`.
		 *
		 * Sets the drawer's components::Overlay visible and restyles at
		 * once.
		 * @return This drawer, for chaining.
		 */
		NavigationDrawer &show(void);

		/**
		 * @brief Hide the drawer and its backdrop.
		 *
		 * Sets the drawer's components::Overlay not visible and restyles at
		 * once.
		 * @return This drawer, for chaining.
		 */
		NavigationDrawer &hide(void);

		/**
		 * @brief Whether the drawer is shown.
		 * @return True when its components::Overlay is visible; false at
		 * creation.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Get the number of destinations.
		 * @return How many entries are destinations.
		 */
		std::size_t getDestinationCount(void) const;

		/**
		 * @brief Get the child entity of a destination: its pressable row.
		 * @param index The index of the destination, sections and dividers
		 * not counted.
		 * @return The identifier of the destination entity, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range.
		 */
		ecs::Entity::Identifier
			getDestinationIdentifier(std::size_t index) const;

		/**
		 * @brief Get the trailing text entity of a destination.
		 * @param index The index of the destination, sections and dividers
		 * not counted.
		 * @return The identifier of the text, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range, the
		 * destination has no trailing text or the variant is `Collapsed`.
		 */
		ecs::Entity::Identifier getRightIdentifier(std::size_t index) const;

		/**
		 * @brief Get the number of section titles built.
		 * @return How many section entries have a title child: sections
		 * with an empty title are not counted, and the count is 0 with the
		 * `Collapsed` variant.
		 */
		std::size_t getSectionCount(void) const;

		/**
		 * @brief Get the text entity of a section title.
		 * @param index The position among the section titles built, from 0.
		 * @return The identifier of the text, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range.
		 */
		ecs::Entity::Identifier getSectionIdentifier(std::size_t index) const;

		/**
		 * @brief Get the number of dividers.
		 * @return How many divider children the drawer has.
		 */
		std::size_t getDividerCount(void) const;

		/**
		 * @brief Get the backdrop: the child that dims what is behind an
		 * open `Modal` drawer.
		 *
		 * The backdrop exists for every variant; it is only shown for
		 * `Modal`.
		 * @return The identifier of the backdrop, or
		 * ecs::Entity::InvalidIdentifier before the children are built.
		 */
		ecs::Entity::Identifier getBackdropIdentifier(void) const;

		private:
		/**
		 * @brief Build the backdrop when it is missing, and a child for
		 * every entry, then wire them to the system.
		 *
		 * Entry children of a previous call are removed first. Does nothing
		 * until the drawer is owned by a shared pointer.
		 */
		void buildDestinations(void);

		private:
		/**
		 * @brief Construct a drawer from (icon, label) pairs: enabled
		 * destinations only, without index-change handler.
		 * @param registry The registry that stores the components.
		 * @param config The surface configuration (pose).
		 * @param variant The variant.
		 * @param destinations The (icon glyph name, label) pair of each
		 * destination, in display order.
		 */
		NavigationDrawer(ecs::ComponentRegistry &registry,
						 const SurfaceConfig &config, Variant variant,
						 const std::vector<std::pair<std::string, std::string>>
							 &destinations);

		/**
		 * @brief Construct a drawer from entries.
		 *
		 * Writes the configuration to components::NavigationDrawer and
		 * styles the sheet. The children are built by build().
		 * @param registry The registry that stores the components.
		 * @param config The surface configuration (pose).
		 * @param variant The variant.
		 * @param entries The destinations, section titles and dividers, in
		 * display order.
		 * @param onIndexChange Called with the new destination index when
		 * the user selects another destination; may be empty.
		 */
		NavigationDrawer(ecs::ComponentRegistry &registry,
						 const SurfaceConfig &config, Variant variant,
						 const std::vector<Entry> &entries,
						 std::function<void(std::size_t)> onIndexChange);

		/**
		 * @brief Build the backdrop and the entry children, then select the
		 * first enabled destination and style the drawer.
		 *
		 * Called once by create(), when the drawer is owned by a shared
		 * pointer and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
