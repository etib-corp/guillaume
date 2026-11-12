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
	 * @brief Checkbox entity: a selectable box with an optional label.
	 *
	 * The checkbox reuses `SurfaceBase` for the box geometry and
	 * `components::Selectable` for the checked state. The three Material
	 * Design states are:
	 * - `Unchecked`: transparent fill with an outline border.
	 * - `Checked`: primary fill with an `check` glyph.
	 * - `Indeterminate`: primary fill with a horizontal stroke
	 *   (`components::Line`).
	 *
	 * The class is a facade without fields: its state is
	 * components::Checkbox and systems::Checkbox styles the box and places
	 * the label. `initialize()` only builds the child entities.
	 *
	 * @see SurfaceBase
	 * @see components::Selectable
	 */
	class Checkbox:
		public SurfaceBase<components::Selectable, components::Line,
						   components::Checkbox>
	{
		public:
		/// Checked state.
		using State = components::Checkbox::State;

		/// Visual variant.
		using Variant = components::Checkbox::Variant;

		/**
		 * @brief Builder used to configure and create `Checkbox` entities.
		 */
		class Builder: public EntityBuilderBase<Checkbox>
		{
			private:
			SurfaceConfig _config;				  ///< Surface configuration.
			State _state { State::Unchecked };	  ///< Checked state.
			Variant _variant { Variant::Enabled };	  ///< Visual variant.
			std::string _label {};					  ///< Label content.

			public:
			/**
			 * @brief Construct a new Checkbox Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Checkbox entity from the current configuration.
			 * @return The newly created Checkbox.
			 */
			std::shared_ptr<Checkbox> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the checkbox pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the checkbox state.
			 * @param state The new state.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withState(State state);

			/**
			 * @brief Set the checkbox variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the checkbox label.
			 * @param label The new label.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &label);

			/**
			 * @brief Create a checkbox using the builder.
			 * @param parent The parent entity.
			 * @param state The checkbox state.
			 * @param variant The checkbox variant.
			 * @param label The checkbox label.
			 * @return The newly created checkbox.
			 */
			std::shared_ptr<Checkbox>
				makeCheckbox(std::shared_ptr<ecs::Entity> parent, State state,
							 Variant variant, const std::string &label);
		};

		public:
		/**
		 * @brief Construct a Checkbox entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param state The checkbox state.
		 * @param variant The visual variant.
		 * @param label The label content.
		 */
		Checkbox(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				 State state, Variant variant, const std::string &label);

		/**
		 * @brief Default destructor.
		 */
		~Checkbox(void) override = default;

		/**
		 * @brief Set the checkbox state.
		 * @param state The new state.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &setState(State state);

		/**
		 * @brief Get the checkbox state.
		 * @return The state.
		 */
		State getState(void) const;

		/**
		 * @brief Set the checkbox variant.
		 * @param variant The new variant.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &setVariant(Variant variant);

		/**
		 * @brief Get the checkbox variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the checkbox label.
		 * @param label The new label.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &setLabel(const std::string &label);

		/**
		 * @brief Whether the checkbox is currently checked.
		 * @return True when checked.
		 */
		bool isChecked(void) const;

		/**
		 * @brief Toggle as a user activation would (indeterminate and
		 * unchecked become checked). Ignored while disabled.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &toggle(void);

		/**
		 * @brief Check whether the checkbox is disabled.
		 * @return True for `Variant::Disabled`.
		 */
		bool isDisabled(void) const;

		/**
		 * @brief Set the handler called when the user toggles the box.
		 * @param onValueChange Handler receiving the new state.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &setOnValueChange(std::function<void(State)> onValueChange);

		/**
		 * @brief Override the checked container color (Paper `color`).
		 * @param color Custom color, or none for the theme color.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &
			setCheckedColor(std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Override the unchecked outline color (Paper
		 * `uncheckedColor`).
		 * @param color Custom color, or none for the theme color.
		 * @return Reference to this checkbox for chaining.
		 */
		Checkbox &setUncheckedColor(
			std::optional<utility::graphic::Color32Bit> color);

		/**
		 * @brief Side of the square interactive target (Paper: 36-40 dp).
		 */
		static constexpr float TouchTarget = components::Checkbox::TouchTarget;

		/**
		 * @brief Gap between the target and the label.
		 */
		static constexpr float LabelGap = components::Checkbox::LabelGap;

		/**
		 * @brief Build the check glyph and label children.
		 */
		void initialize(void) override;
	};

}	 // namespace guillaume::entities
