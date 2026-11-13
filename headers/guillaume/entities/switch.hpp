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

#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/switch.hpp"
#include "guillaume/components/value.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief An on/off toggle: a rounded track with a thumb that slides to
	 * the right when on, an optional icon on the thumb and an optional
	 * label next to the track.
	 *
	 * Its state lives in components::Switch (variant, icon glyph, label,
	 * value-change handler, identifiers of its children) and in
	 * components::Value, which holds 0 when off and 1 when on.
	 * systems::Switch colors the track, places the thumb, icon and label
	 * and turns input into toggles; it runs each frame in the Layout
	 * phase. The class itself holds no field: its setters write the
	 * components and restyle at once.
	 *
	 * @code
	 * Switch::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto wifi = builder.makeSwitch(parent, Switch::Variant::Enabled, false,
	 * 	"", "Wi-Fi");
	 * wifi->setOnValueChange([](bool checked) {
	 * 	// called when the user toggles the switch
	 * });
	 * @endcode
	 *
	 * `Switch::create(registry, config, variant, checked, iconGlyph, label)`
	 * builds the same widget and its children without registering it in an
	 * entity registry.
	 *
	 * Watch out:
	 * - setChecked() does not call the value-change handler; only a user
	 *   activation and toggle() do.
	 * - The icon and the label are created only at construction, when
	 *   their text is not empty. setLabel() changes the text of an
	 *   existing label but does not create one.
	 * - The icon is shown whenever a glyph name is given, whatever the
	 *   variant: `WithIcon` is styled like `Enabled`.
	 * - The Bound of the switch is its track (52 x 32 dp with the builder
	 *   defaults). The label is drawn `LabelGap` dp to the right of it,
	 *   outside the Bound, so a parent layout does not reserve room for
	 *   it.
	 * - systems::Switch overwrites the track and stroke colors and the
	 *   colors of the children every frame; SurfaceBase::setColor() does
	 *   not last.
	 * - The switch owns its thumb, icon and label.
	 *
	 * @see components::Switch, systems::Switch, Checkbox
	 */
	class Switch:
		public SurfaceBase<
			components::Value, components::MouseButtonInteraction,
			components::HandButtonInteraction, components::Switch>
	{
		public:
		/// Whether the switch reacts to input: `Enabled`, `Disabled`
		/// (dimmed, ignores input) or `WithIcon` (styled like `Enabled`).
		using Variant = components::Switch::Variant;

		/**
		 * @brief Configures Switch widgets and registers them in the
		 * scene's entity registry.
		 */
		class Builder: public EntityBuilderBase<Switch>
		{
			private:
			SurfaceConfig _config;	  ///< Pose, size and radius of the track.
			Variant _variant { Variant::Enabled };	  ///< Enabled or disabled.
			bool _checked { false };	  ///< Whether the switch starts on.
			std::string _iconGlyph {};	  ///< Material Symbols name of the
										  ///< thumb icon (empty: no icon).
			std::string _label {};		  ///< Label text (empty: no label).

			public:
			/**
			 * @brief Construct a builder holding the defaults of reset().
			 * @param componentRegistry The registry the components of the
			 * built switches are stored in.
			 * @param entityRegistry The registry that owns the switches
			 * built by `makeSwitch`.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a switch and its children from the current
			 * configuration, without registering it or giving it a parent.
			 * @return The new switch.
			 */
			std::shared_ptr<Switch> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: default pose, the `Enabled`
			 * variant, off, no icon, no label, and a 52 x 32 dp track with
			 * fully rounded ends.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the switch is.
			 * @param pose The pose of the top-left corner of the track. A
			 * default-constructed pose by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Choose whether the switch reacts to input.
			 * @param variant `Enabled`, `Disabled` or `WithIcon`.
			 * `Enabled` by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Choose whether the switch starts on.
			 * @param checked True for on, false for off. Off by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withChecked(bool checked);

			/**
			 * @brief Draw an icon on the thumb.
			 * @param iconGlyph The Material Symbols name of the icon, such
			 * as `"check"`. Empty by default, which builds no icon.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the text shown to the right of the track.
			 * @param label The label. It is also the accessibility label.
			 * Empty by default, which builds no label child.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withLabel(const std::string &label);

			/**
			 * @brief Build a switch and its children, register it under
			 * `parent` and reset the builder.
			 * @param parent The entity the switch is attached to, or
			 * nullptr for a root entity.
			 * @param variant `Enabled`, `Disabled` or `WithIcon`.
			 * @param checked True to start on, false to start off.
			 * @param iconGlyph The Material Symbols name of the thumb icon,
			 * or an empty string for no icon.
			 * @param label The text shown to the right of the track, or an
			 * empty string for no label.
			 * @return The registered switch.
			 */
			std::shared_ptr<Switch>
				makeSwitch(std::shared_ptr<ecs::Entity> parent, Variant variant,
						   bool checked, const std::string &iconGlyph,
						   const std::string &label);
		};

		public:
		/**
		 * @brief Build a switch without registering it in an entity
		 * registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * `(registry, config, variant, checked, iconGlyph, label)`. It then
		 * calls build(), which creates the thumb and, when their text is
		 * not empty, the icon and the label.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new switch. The caller owns it; the switch owns its
		 * children.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Switch> create(Arguments &&...arguments)
		{
			std::shared_ptr<Switch> entity(
				new Switch(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~Switch(void) override = default;

		/**
		 * @brief Enable or disable the switch. Restyles at once.
		 * @param variant `Enabled` or `WithIcon` for a switch that reacts
		 * to input, `Disabled` for a dimmed switch that ignores it.
		 * @return Reference to this switch for chaining.
		 */
		Switch &setVariant(Variant variant);

		/**
		 * @brief Get the variant of the switch.
		 * @return The variant stored in components::Switch.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Turn the switch on or off from code.
		 *
		 * Writes components::Value; when the state changes, the thumb
		 * moves and the switch is restyled at once. It works on a disabled
		 * switch too. The value-change handler is not called.
		 * @param checked True for on, false for off.
		 * @return Reference to this switch for chaining.
		 */
		Switch &setChecked(bool checked);

		/**
		 * @brief Whether the switch is on.
		 * @return True when on, that is when its Value is at least 0.5.
		 */
		bool isChecked(void) const;

		/**
		 * @brief Flip the checked state, as a user activation would.
		 *
		 * Does nothing while the switch is disabled. Calls the value-change
		 * handler with the new state.
		 *
		 * @return Reference to this switch for chaining.
		 */
		Switch &toggle(void);

		/**
		 * @brief Set the handler called when the user toggles the switch
		 * (Paper `onValueChange`).
		 *
		 * It is called after the state changed, on a user activation or
		 * toggle(), never on setChecked().
		 * @param onValueChange The function to call, receiving the new
		 * state (true for on). An empty function removes the handler.
		 * @return Reference to this switch for chaining.
		 */
		Switch &setOnValueChange(std::function<void(bool)> onValueChange);

		/**
		 * @brief Check whether the switch ignores input.
		 * @return True for `Variant::Disabled`.
		 */
		bool isDisabled(void) const;

		/**
		 * @brief Gap between the track and the label, in dp.
		 */
		static constexpr float LabelGap = components::Switch::LabelGap;

		/**
		 * @brief Change the text of the label. Restyles at once.
		 *
		 * The new text is also the accessibility label. It is drawn only
		 * when the switch was built with a non-empty label: this function
		 * does not create the label child.
		 * @param label The new label.
		 * @return Reference to this switch for chaining.
		 */
		Switch &setLabel(const std::string &label);

		private:
		/**
		 * @brief Construct a switch, wire its input and style it.
		 * @param registry The registry the components are stored in.
		 * @param config The pose, size and corner radius of the track. Its
		 * color is replaced by systems::Switch.
		 * @param variant `Enabled`, `Disabled` or `WithIcon`.
		 * @param checked True to start on, false to start off.
		 * @param iconGlyph The Material Symbols name of the thumb icon, or
		 * an empty string for no icon.
		 * @param label The label text, or an empty string for no label.
		 */
		Switch(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, bool checked, const std::string &iconGlyph,
			   const std::string &label);

		/**
		 * @brief Create the thumb and, when their text is not empty, the
		 * icon and the label, then place and style them.
		 *
		 * Called once by create(), when the switch is held by a shared
		 * pointer and can be given as parent to its children. The children
		 * are registered in the switch itself, which owns them; none is a
		 * layout child, since systems::Switch places them.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
