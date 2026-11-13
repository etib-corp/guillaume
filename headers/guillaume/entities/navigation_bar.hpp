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
	 * @brief A bottom bar of destinations (icon, label, optional badge) of
	 * which exactly one is selected.
	 *
	 * Its state lives in components::NavigationBar (variant, destination
	 * data, destination child identifiers, callbacks);
	 * systems::NavigationBar colors the bar, sizes the destinations and
	 * places them in a row each frame, in the Layout phase. Which
	 * destination is selected is stored in the components::Selectable of
	 * each destination child; the bar's components::SelectionGroup keeps
	 * the selection single and never empty. The class itself holds no
	 * field.
	 *
	 * @code
	 * NavigationBar::Builder builder(getComponentRegistry(),
	 * 	getEntityRegistry());
	 * auto bar = builder
	 * 	.withOnIndexChange([](std::size_t index) {
	 * 		// called when the user selects another destination
	 * 	})
	 * 	.makeNavigationBar(parent, NavigationBar::Variant::WithLabels,
	 * 		{ { "home", "Home" }, { "settings", "Settings" } });
	 * bar->setBadge(1, "3");
	 * @endcode
	 *
	 * `NavigationBar::create(registry, config, variant, destinations,
	 * onIndexChange)` builds the same widget without registering it in an
	 * entity registry.
	 *
	 * Things to know:
	 * - Indexes are positions in the order the destinations were added,
	 *   starting at 0. The first destination is selected at creation.
	 * - select() does not call the index-change handler; only a press by
	 *   the user on another destination does.
	 * - addDestination() selects the first destination again.
	 * - The bar is the entity registry that owns its destination children
	 *   (detail::BarDestination). Destinations are only appended: the
	 *   identifiers returned by the getters stay valid.
	 * - The Builder gives the bar 360 dp by components::NavigationBar::Height
	 *   (80 dp). The destinations share the width equally; their size and
	 *   the bar's color and layout children are overwritten each frame.
	 * - With `Variant::WithoutLabels`, only the selected destination shows
	 *   its label.
	 *
	 * @see components::NavigationBar, systems::NavigationBar,
	 * NavigationDrawer
	 */
	class NavigationBar:
		public SurfaceBase<components::SelectionGroup,
						   components::NavigationBar>
	{
		public:
		/// Variant: `WithLabels` shows every label, `WithoutLabels` only
		/// the label of the selected destination.
		using Variant = components::NavigationBar::Variant;

		/// One destination: icon glyph name, label, glyph name of the icon
		/// shown while selected, badge label.
		using Destination = components::NavigationBar::Destination;

		/**
		 * @brief Configures a NavigationBar, creates it and registers it in
		 * an entity registry.
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
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry that stores the
			 * components of the bars built.
			 * @param entityRegistry The registry the bars are added to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a navigation bar and its destinations from the
			 * current configuration, without registering it or setting its
			 * parent.
			 * @return The new navigation bar.
			 */
			std::shared_ptr<NavigationBar> buildEntity(void) override;

			/**
			 * @brief Restore the defaults.
			 *
			 * Defaults: `WithLabels` variant, no destination, no
			 * index-change handler, default pose, 360 dp wide, 80 dp high
			 * (components::NavigationBar::Height), 8 dp of padding.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the bar is placed.
			 * @param pose The pose of the bar. Default: a
			 * default-constructed pose.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the variant.
			 * @param variant `WithLabels` (the default) to show every
			 * label, `WithoutLabels` to show only the label of the selected
			 * destination.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Append a destination after the ones already added.
			 * @param iconGlyph The glyph name of the icon, such as
			 * `"home"`.
			 * @param label The text shown under the icon.
			 * @param focusedIconGlyph The glyph name of the icon shown
			 * while the destination is selected; empty to keep `iconGlyph`.
			 * @param badge The badge label; empty for no badge.
			 * @return This builder, for chaining.
			 */
			Builder &addDestination(const std::string &iconGlyph,
									const std::string &label,
									const std::string &focusedIconGlyph = "",
									const std::string &badge			= "");

			/**
			 * @brief Set the handler called when the user selects another
			 * destination (Paper `onIndexChange`).
			 * @param onIndexChange Called with the index of the newly
			 * selected destination. Not called by select(). Default: none.
			 * @return This builder, for chaining.
			 */
			Builder &withOnIndexChange(
				std::function<void(std::size_t)> onIndexChange);

			/**
			 * @brief Build a navigation bar, register it under `parent` and
			 * reset the builder.
			 *
			 * The destinations given here come after the ones already added
			 * with addDestination().
			 * @param parent The entity the bar becomes a child of, or
			 * nullptr for a root entity.
			 * @param variant The variant of the bar.
			 * @param destinations The (icon glyph name, label) pair of each
			 * destination to append, in display order.
			 * @return The new navigation bar.
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
		 *
		 * Existing destination children are kept. Does nothing until the
		 * bar is owned by a shared pointer.
		 */
		void buildDestinations(void);

		public:
		/**
		 * @brief Create a navigation bar and its destination children,
		 * without registering it in an entity registry.
		 *
		 * The constructors are private because the children need the bar
		 * to be owned by a shared pointer before they can take it as
		 * parent; create() constructs the bar, builds one child per
		 * destination and selects the first one.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments Forwarded to a private constructor: either
		 * (registry, config, variant, destinations, onIndexChange) with
		 * `Destination` values, or (registry, config, variant, destinations)
		 * with (icon glyph name, label) pairs.
		 * @return The new navigation bar.
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
		 * @brief Change the variant. Restyles at once.
		 * @param variant `WithLabels` to show every label, `WithoutLabels`
		 * to show only the label of the selected destination.
		 * @return This bar, for chaining.
		 */
		NavigationBar &setVariant(Variant variant);

		/**
		 * @brief Get the variant.
		 * @return The variant stored in components::NavigationBar.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Append a destination without focused icon or badge.
		 *
		 * Same effects as the overload taking a `Destination`.
		 * @param iconGlyph The glyph name of the icon.
		 * @param label The text shown under the icon.
		 * @return This bar, for chaining.
		 */
		NavigationBar &addDestination(const std::string &iconGlyph,
									  const std::string &label);

		/**
		 * @brief Append a destination.
		 *
		 * Builds the child entity of the new destination, selects the
		 * first destination of the bar again and restyles at once. The
		 * children of the existing destinations are kept.
		 * @param destination The icon, label, focused icon and badge of the
		 * new destination.
		 * @return This bar, for chaining.
		 */
		NavigationBar &addDestination(const Destination &destination);

		/**
		 * @brief Change the badge label of a destination.
		 *
		 * Builds the badge child the first time the label is not empty,
		 * then restyles the destination at once. An empty label turns an
		 * existing badge into a dot; it does not remove it.
		 * @param index The index of the destination; nothing happens when
		 * it is out of range.
		 * @param badge The badge label.
		 * @return This bar, for chaining.
		 */
		NavigationBar &setBadge(std::size_t index, const std::string &badge);

		/**
		 * @brief Set the handler called when the user selects another
		 * destination (Paper `onIndexChange`).
		 * @param onIndexChange Called with the index of the newly selected
		 * destination, after the selection changed. Not called by select()
		 * nor when the user presses the selected destination.
		 * @return This bar, for chaining.
		 */
		NavigationBar &
			setOnIndexChange(std::function<void(std::size_t)> onIndexChange);

		/**
		 * @brief Set the handler called on every destination press, even on
		 * the selected one (Paper `onTabPress`).
		 * @param onTabPress Called with the index of the pressed
		 * destination, before the selection changes.
		 * @return This bar, for chaining.
		 */
		NavigationBar &
			setOnTabPress(std::function<void(std::size_t)> onTabPress);

		/**
		 * @brief Get the active-indicator pill of a destination: the
		 * rounded shape behind the icon.
		 * @param index The index of the destination.
		 * @return The identifier of the pill, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range.
		 */
		ecs::Entity::Identifier getIndicatorIdentifier(std::size_t index) const;

		/**
		 * @brief Get the icon entity of a destination.
		 * @param index The index of the destination.
		 * @return The identifier of the icon, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range or the
		 * destination has no icon.
		 */
		ecs::Entity::Identifier getIconIdentifier(std::size_t index) const;

		/**
		 * @brief Get the label entity of a destination.
		 * @param index The index of the destination.
		 * @return The identifier of the label text, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range or the
		 * destination has no label.
		 */
		ecs::Entity::Identifier getLabelIdentifier(std::size_t index) const;

		/**
		 * @brief Get the badge entity of a destination.
		 * @param index The index of the destination.
		 * @return The identifier of the badge, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range or the
		 * destination never had a badge label.
		 */
		ecs::Entity::Identifier getBadgeIdentifier(std::size_t index) const;

		/**
		 * @brief Select a destination and deselect the others.
		 *
		 * Restyles at once. Does not call the index-change handler.
		 * @param index The index of the destination to select; nothing
		 * happens when it is out of range.
		 * @return This bar, for chaining.
		 */
		NavigationBar &select(std::size_t index);

		/**
		 * @brief Get the index of the selected destination.
		 * @return The index of the selected destination, or 0 when the bar
		 * has no destination.
		 */
		std::size_t getSelectedIndex(void) const;

		/**
		 * @brief Get the number of destinations.
		 * @return How many destinations the bar has.
		 */
		std::size_t getDestinationCount(void) const;

		/**
		 * @brief Get the child entity of a destination: the pressable area
		 * holding its indicator, icon and label.
		 * @param index The index of the destination.
		 * @return The identifier of the destination entity, or
		 * ecs::Entity::InvalidIdentifier when `index` is out of range.
		 */
		ecs::Entity::Identifier
			getDestinationIdentifier(std::size_t index) const;

		private:
		/**
		 * @brief Construct a navigation bar from (icon, label) pairs,
		 * without focused icon, badge or index-change handler.
		 * @param registry The registry that stores the components.
		 * @param config The surface configuration (pose, size, padding).
		 * @param variant The variant.
		 * @param destinations The (icon glyph name, label) pair of each
		 * destination, in display order.
		 */
		NavigationBar(ecs::ComponentRegistry &registry,
					  const SurfaceConfig &config, Variant variant,
					  const std::vector<std::pair<std::string, std::string>>
						  &destinations);

		/**
		 * @brief Construct a navigation bar from full destinations.
		 *
		 * Writes the configuration to components::NavigationBar and styles
		 * the bar. The destination children are built by build().
		 * @param registry The registry that stores the components.
		 * @param config The surface configuration (pose, size, padding).
		 * @param variant The variant.
		 * @param destinations The destinations, in display order.
		 * @param onIndexChange Called with the new index when the user
		 * selects another destination; may be empty.
		 */
		NavigationBar(ecs::ComponentRegistry &registry,
					  const SurfaceConfig &config, Variant variant,
					  const std::vector<Destination> &destinations,
					  std::function<void(std::size_t)> onIndexChange);

		/**
		 * @brief Build the destination children, select the first one and
		 * style the bar.
		 *
		 * Called once by create(), when the bar is owned by a shared
		 * pointer and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
