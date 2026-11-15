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

#include <utility>
#include <functional>
#include <memory>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/chip.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A compact, 32 dp high pill with a label and optional icons,
	 * used as an action, a filter, an entered value or a suggestion.
	 *
	 * Its state lives in components::Chip (variant, mode, label, flags,
	 * callbacks, child identifiers), its selected state in
	 * components::Selectable and its enabled state in
	 * components::StyleState; systems::Chip colors the chip and its
	 * children and arranges them in a row each frame, in the Layout phase.
	 * The class itself holds no field: its setters write the components
	 * and restyle at once.
	 *
	 * The variant decides which children are built:
	 * - `Assist`: a leading icon (a default glyph when none is given) and
	 *   the label.
	 * - `Filter`: the label, a leading icon when a glyph is given, and a
	 *   check icon that takes the place of the leading icon while the chip
	 *   is selected. A press toggles the selection.
	 * - `Input`: the label, a leading icon when a glyph is given, and a
	 *   trailing close icon that calls the close handler.
	 * - `Suggestion`: the label, and a leading icon when a glyph is given.
	 *
	 * @code
	 * Chip::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto chip = builder.makeChip(parent, Chip::Variant::Filter, "Vegan",
	 * 	"", false);
	 * chip->setOnPress([] {
	 * 	// called when the user presses the chip
	 * });
	 * @endcode
	 *
	 * `Chip::create(registry, config, variant, label, iconGlyph, selected)`
	 * builds the same widget without registering it in an entity registry;
	 * `config` is the SurfaceConfig of the pill (pose, padding, height).
	 *
	 * The chip owns its icon and label children: it is the entity registry
	 * they are registered in. The children are built once, from the variant
	 * given at creation: setVariant() changes the behavior and the colors
	 * but adds no icon. The chip's Color, Borders, Elevation and layout
	 * padding are written each frame, so values set by hand on them are
	 * overwritten. setSelected() does not call the press handler; only a
	 * user press does. getChildren() returns the children currently laid
	 * out, read from the chip's components::Layout.
	 *
	 * @see components::Chip, systems::Chip, SurfaceBase
	 */
	class Chip:
		public SurfaceBase<components::Selectable, components::Elevation,
						   components::Chip>
	{
		public:
		/// Chip variant: Assist, Filter, Input or Suggestion (see
		/// components::Chip::Variant).
		using Variant = components::Chip::Variant;

		/// Paper chip mode: Flat (tonal fill, the default) or Outlined.
		using Mode = components::Chip::Mode;

		/**
		 * @brief Configures, creates and registers Chip widgets.
		 *
		 * Set options with the `with...` methods then call registerEntity(),
		 * or use makeChip() to do both in one call.
		 */
		class Builder: public EntityBuilderBase<Chip>
		{
			private:
			SurfaceConfig _config;	  ///< Pose, layout and size of the pill.
			Variant _variant { Variant::Assist };	 ///< Chip variant.
			std::string _label {};					 ///< Label text.
			std::string
				_iconGlyph {};	  ///< Leading icon glyph name; may be empty.
			bool _selected { false };	 ///< Whether the chip starts selected.

			public:
			/**
			 * @brief Construct a builder with the default configuration (see
			 * reset()).
			 * @param componentRegistry The registry that stores the
			 * components of the chips to build.
			 * @param entityRegistry The registry the built chips are added
			 * to (usually the scene's).
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a Chip, with its icon and label children, from
			 * the current configuration, without registering it.
			 *
			 * registerEntity() calls it, then sets the parent, registers the
			 * chip and resets the builder.
			 * @return The new chip.
			 */
			std::shared_ptr<Chip> buildEntity(void) override;

			/**
			 * @brief Restore the default configuration.
			 *
			 * Defaults: Variant::Assist, empty label, no icon glyph, not
			 * selected, default pose. The pill is a horizontal row with
			 * centered content, 8 dp between children, 12 dp of padding, a
			 * fixed height of 32 dp and the theme's small corner radius.
			 */
			void reset(void) override;

			/**
			 * @brief Set the position and orientation of the chip.
			 * @param pose The pose written to the chip's Transform. Default:
			 * a default-constructed pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the variant, which decides the children built and
			 * whether a press toggles the selection.
			 * @param variant The variant. Default: Variant::Assist.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the label text.
			 * @param label The text shown in the chip, also used as its
			 * accessibility label. Default: empty.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &label);

			/**
			 * @brief Set the leading icon.
			 * @param iconGlyph The glyph name of the leading icon. Default:
			 * empty, which means no leading icon, except for an Assist chip
			 * which then shows components::Chip::DefaultAssistGlyph.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyph);

			/**
			 * @brief Choose whether the chip starts selected.
			 * @param selected True for a selected chip. Default: false.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSelected(bool selected);

			/**
			 * @brief Build a chip, register it under `parent` and reset the
			 * builder.
			 *
			 * A pose set before the call with withPose() also applies.
			 * @param parent The entity the chip becomes a child of.
			 * @param variant The variant.
			 * @param label The text shown in the chip.
			 * @param iconGlyph The glyph name of the leading icon; may be
			 * empty.
			 * @param selected True for a chip that starts selected.
			 * @return The new chip, registered in the entity registry.
			 */
			std::shared_ptr<Chip> makeChip(std::shared_ptr<ecs::Entity> parent,
										   Variant variant,
										   const std::string &label,
										   const std::string &iconGlyph,
										   bool selected);
		};

		/**
		 * @brief Create a chip and its icon and label children, without
		 * registering it in an entity registry.
		 *
		 * The constructor is private because the children need a chip
		 * already held by a `std::shared_ptr` as parent: create() constructs
		 * the chip then calls build(), which creates them.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments The arguments forwarded to the constructor:
		 * (registry, config, variant, label, iconGlyph, selected).
		 * @return The new chip.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Chip> create(Arguments &&...arguments)
		{
			std::shared_ptr<Chip> entity(
				new Chip(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~Chip(void) override;

		/**
		 * @brief Set the variant. Restyles at once.
		 *
		 * Only the behavior follows (a Filter chip toggles its selection on
		 * a press): the icon children were built for the variant given at
		 * creation and none is added or removed.
		 * @param variant The new variant.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setVariant(Variant variant);

		/**
		 * @brief Get the variant.
		 * @return The variant stored in components::Chip.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the label text. Restyles at once.
		 * @param label The text shown in the chip, also used as its
		 * accessibility label.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setLabel(const std::string &label);

		/**
		 * @brief Select or deselect the chip, whatever its variant.
		 * Restyles at once.
		 *
		 * The press handler is not called.
		 * @param selected True to select the chip.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setSelected(bool selected);

		/**
		 * @brief Check whether the chip is selected.
		 * @return True when its components::Selectable is selected.
		 */
		bool isSelected(void) const;

		/**
		 * @brief Set the mode. Restyles at once.
		 * @param mode Mode::Flat for a tonal fill, Mode::Outlined for a
		 * transparent chip with a 1 dp outline until it is selected.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setMode(Mode mode);

		/**
		 * @brief Get the mode.
		 * @return Mode::Flat (the default) or Mode::Outlined.
		 */
		Mode getMode(void) const;

		/**
		 * @brief Enable or disable the chip. Restyles at once.
		 *
		 * A disabled chip is drawn with the disabled colors and calls
		 * neither the press handler nor the close handler.
		 * @param disabled True to disable the chip, false to enable it.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setDisabled(bool disabled);

		/**
		 * @brief Check whether the chip is disabled.
		 * @return True when the chip ignores input.
		 */
		bool isDisabled(void) const;

		/**
		 * @brief Use the compact padding (Paper `compact`). Restyles at
		 * once.
		 * @param compact True for 8 dp of padding around the content, false
		 * for 12 dp (the default).
		 * @return Reference to this chip for chaining.
		 */
		Chip &setCompact(bool compact);

		/**
		 * @brief Paint a level 1 shadow under the chip (Paper `elevated`).
		 * Restyles at once.
		 * @param elevated True for a shadow, false for none (the default). A
		 * disabled chip has no shadow.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setElevated(bool elevated);

		/**
		 * @brief Darken the chip with a pressed state layer while it is
		 * selected (Paper `showSelectedOverlay`). Restyles at once.
		 * @param show True to overlay the layer while selected, false not to
		 * (the default).
		 * @return Reference to this chip for chaining.
		 */
		Chip &setShowSelectedOverlay(bool show);

		/**
		 * @brief Show the check icon while the chip is selected (Paper
		 * `showSelectedCheck`). Restyles at once.
		 *
		 * Only a chip created as a Filter chip has a check icon.
		 * @param show True to show the check while selected (the default),
		 * false to keep the leading icon.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setShowSelectedCheck(bool show);

		/**
		 * @brief Set the handler called when the user presses the chip
		 * (Paper `onPress`).
		 *
		 * For a Filter chip it is called after the selection was toggled.
		 * @param onPress The function to call. Pass an empty function to
		 * remove the handler.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setOnPress(std::function<void(void)> onPress);

		/**
		 * @brief Set the handler called when the user presses the trailing
		 * close icon (Paper `onClose`).
		 *
		 * The handler decides what closing means; the chip does not remove
		 * itself. Only a chip created as an Input chip has a close icon:
		 * setting a handler on another chip adds none.
		 * @param onClose The function to call. Pass an empty function to
		 * remove the handler.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setOnClose(std::function<void(void)> onClose);

		/**
		 * @brief Get the identifier of the trailing close icon child.
		 * @return The identifier of the close icon, or
		 * ecs::Entity::InvalidIdentifier when the chip has none.
		 */
		ecs::Entity::Identifier getCloseIconIdentifier(void) const;

		private:
		/**
		 * @brief Construct a chip without its children; create() calls it
		 * then build().
		 *
		 * Fills components::Chip and components::Selectable and installs
		 * systems::Chip on the entity.
		 * @param registry The registry that stores the chip's components.
		 * @param config The pose, layout and size of the pill.
		 * @param variant The variant, which decides the children built.
		 * @param label The text shown in the chip.
		 * @param iconGlyph The glyph name of the leading icon; may be empty.
		 * @param selected True for a chip that starts selected.
		 */
		Chip(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			 Variant variant, const std::string &label,
			 const std::string &iconGlyph, bool selected);

		/**
		 * @brief Create the icon and label children the variant needs and
		 * record their identifiers in components::Chip.
		 *
		 * Called once by create(), when the chip is held by a
		 * `std::shared_ptr` and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
