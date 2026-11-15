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
#include <optional>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/checkbox.hpp"
#include "guillaume/components/line.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A box the user checks, unchecks or leaves indeterminate, with
	 * an optional label next to it.
	 *
	 * Its state lives in components::Checkbox (state, variant, label,
	 * colors, callback, child identifiers); systems::Checkbox styles the
	 * box and places the label each frame, in the Layout phase. The class
	 * itself holds no field: its setters write the component and restyle at
	 * once.
	 *
	 * The three states are drawn as follows:
	 * - `Unchecked`: transparent box with an outline.
	 * - `Checked`: filled box with a `check` glyph.
	 * - `Indeterminate`: filled box with a horizontal stroke
	 *   (components::Line).
	 *
	 * @code
	 * Checkbox::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto checkbox = builder.makeCheckbox(parent, Checkbox::State::Unchecked,
	 * 	Checkbox::Variant::Enabled, "Accept the terms");
	 * checkbox->setOnValueChange([](Checkbox::State state) {
	 * 	// called when the user toggles the box
	 * });
	 * @endcode
	 *
	 * `Checkbox::create(registry, config, state, variant, label)` builds the
	 * same widget without registering it in an entity registry; `config` is
	 * the SurfaceConfig of the box (pose, size).
	 *
	 * The checkbox owns its check glyph and label children: it is the
	 * entity registry they are registered in. The label child is built only
	 * when the label given at creation is not empty. The box is 18 dp wide
	 * and answers input on a TouchTarget square centered on it. Its Color,
	 * Borders and enabled state are written each frame from the state and
	 * the variant, so values set by hand on them are overwritten.
	 * setState() does not call the value-change handler; a user activation
	 * and toggle() do.
	 *
	 * @see components::Checkbox, systems::Checkbox, RadioButton
	 */
	class Checkbox:
		public SurfaceBase<components::Selectable, components::Line,
						   components::Checkbox>
	{
		public:
		/// Checked state: Unchecked, Checked or Indeterminate.
		using State = components::Checkbox::State;

		/// Visual variant: Enabled, Disabled (ignores input) or Error
		/// (error colors).
		using Variant = components::Checkbox::Variant;

		/**
		 * @brief Configures, creates and registers Checkbox widgets.
		 *
		 * Set options with the `with...` methods then call registerEntity(),
		 * or use makeCheckbox() to do both in one call.
		 */
		class Builder: public EntityBuilderBase<Checkbox>
		{
			private:
			SurfaceConfig _config;	  ///< Pose, layout and size of the box.
			State _state { State::Unchecked };		  ///< Initial state.
			Variant _variant { Variant::Enabled };	  ///< Visual variant.
			std::string _label {};	  ///< Label text; empty for no label.

			public:
			/**
			 * @brief Construct a builder with the default configuration (see
			 * reset()).
			 * @param componentRegistry The registry that stores the
			 * components of the checkboxes to build.
			 * @param entityRegistry The registry the built checkboxes are
			 * added to (usually the scene's).
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a Checkbox, with its check glyph and label
			 * children, from the current configuration, without registering
			 * it.
			 *
			 * registerEntity() calls it, then sets the parent, registers the
			 * checkbox and resets the builder.
			 * @return The new checkbox.
			 */
			std::shared_ptr<Checkbox> buildEntity(void) override;

			/**
			 * @brief Restore the default configuration.
			 *
			 * Defaults: State::Unchecked, Variant::Enabled, no label,
			 * default pose. The box is an 18 dp square with a 2 dp corner
			 * radius and no padding, and centers its content.
			 */
			void reset(void) override;

			/**
			 * @brief Set the position and orientation of the box.
			 * @param pose The pose written to the checkbox's Transform.
			 * Default: a default-constructed pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the state the checkbox starts in.
			 * @param state The initial state. Default: State::Unchecked.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withState(State state);

			/**
			 * @brief Set the visual variant.
			 * @param variant Enabled, Disabled (ignores input) or Error
			 * (error colors). Default: Variant::Enabled.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the label shown to the right of the box.
			 * @param label The label text, also used as the accessibility
			 * label. Default: empty, no label child is built.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &label);

			/**
			 * @brief Build a checkbox, register it under `parent` and reset
			 * the builder.
			 *
			 * A pose set before the call with withPose() also applies.
			 * @param parent The entity the checkbox becomes a child of.
			 * @param state The state the checkbox starts in.
			 * @param variant Enabled, Disabled or Error.
			 * @param label The label text; empty for a checkbox without
			 * label.
			 * @return The new checkbox, registered in the entity registry.
			 */
			std::shared_ptr<Checkbox>
				makeCheckbox(std::shared_ptr<ecs::Entity> parent, State state,
							 Variant variant, const std::string &label);
		};

		public:
		/**
		 * @brief Create a checkbox and its check glyph and label children,
		 * without registering it in an entity registry.
		 *
		 * The constructor is private because the children need a checkbox
		 * already held by a `std::shared_ptr` as parent: create() constructs
		 * the checkbox then calls build(), which creates them.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments The arguments forwarded to the constructor:
		 * (registry, config, state, variant, label).
		 * @return The new checkbox.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Checkbox> create(Arguments &&...arguments)
		{
			std::shared_ptr<Checkbox> entity(
				new Checkbox(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~Checkbox(void) override = default;

		/**
		 * @brief Set the state. Restyles at once.
		 *
		 * The value-change handler is not called.
		 * @param state The new state: Unchecked, Checked or Indeterminate.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &setState(State state);

		/**
		 * @brief Get the state.
		 * @return The state stored in components::Checkbox.
		 */
		State getState(void) const;

		/**
		 * @brief Set the visual variant. Restyles at once.
		 * @param variant Enabled, Disabled (ignores input) or Error (error
		 * colors).
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &setVariant(Variant variant);

		/**
		 * @brief Get the visual variant.
		 * @return The variant stored in components::Checkbox.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the label text and the accessibility label. Restyles at
		 * once.
		 *
		 * The text on screen changes only when the checkbox was created with
		 * a label: no label child is built here.
		 * @param label The new label text.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &setLabel(const std::string &label);

		/**
		 * @brief Check whether the checkbox's components::Selectable is
		 * selected.
		 *
		 * setState() and toggle() select it for every state but Unchecked,
		 * so an indeterminate checkbox also reports true. Use getState() to
		 * tell the three states apart.
		 * @return True when selected.
		 */
		bool isChecked(void) const;

		/**
		 * @brief Toggle as a user activation would: a checked box becomes
		 * unchecked, an unchecked or indeterminate box becomes checked.
		 * Restyles at once.
		 *
		 * Calls the value-change handler with the new state. Does nothing
		 * while the checkbox is disabled.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &toggle(void);

		/**
		 * @brief Check whether the checkbox ignores input.
		 * @return True for `Variant::Disabled`.
		 */
		bool isDisabled(void) const;

		/**
		 * @brief Set the handler called when the user toggles the box, or
		 * when toggle() is called.
		 * @param onValueChange The function that receives the new state.
		 * Pass an empty function to remove the handler.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &setOnValueChange(std::function<void(State)> onValueChange);

		/**
		 * @brief Override the fill color of the checked and indeterminate
		 * box (Paper `color`). Restyles at once.
		 *
		 * The Error and Disabled variants keep their own colors.
		 * @param color The custom color, or `std::nullopt` for the theme
		 * color.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &
			setCheckedColor(std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Override the outline color of the unchecked box (Paper
		 * `uncheckedColor`). Restyles at once.
		 *
		 * The Error and Disabled variants keep their own colors.
		 * @param color The custom color, or `std::nullopt` for the theme
		 * color.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &setUncheckedColor(
			std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Side of the square that answers input, centered on the box,
		 * in dp.
		 */
		static constexpr float TouchTarget = components::Checkbox::TouchTarget;

		/**
		 * @brief Gap between the touch target and the label, in dp.
		 */
		static constexpr float LabelGap = components::Checkbox::LabelGap;

		private:
		/**
		 * @brief Construct a checkbox without its children; create() calls
		 * it then build().
		 *
		 * Fills components::Checkbox and installs systems::Checkbox on the
		 * entity.
		 * @param registry The registry that stores the checkbox's
		 * components.
		 * @param config The pose, layout and size of the box.
		 * @param state The state the checkbox starts in.
		 * @param variant Enabled, Disabled or Error.
		 * @param label The label text; empty for no label.
		 */
		Checkbox(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				 State state, Variant variant, const std::string &label);

		/**
		 * @brief Create the check glyph child and, when the label is not
		 * empty, the label child, and record their identifiers in
		 * components::Checkbox.
		 *
		 * Called once by create(), when the checkbox is held by a
		 * `std::shared_ptr` and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
