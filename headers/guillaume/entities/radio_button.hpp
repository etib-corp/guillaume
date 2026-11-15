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

#include "guillaume/components/radio_button.hpp"
#include "guillaume/components/ring.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A circle the user selects to pick one option, with an optional
	 * label next to it.
	 *
	 * Its state lives in components::RadioButton (variant, label, callback,
	 * child identifiers) and its selection in components::Selectable;
	 * systems::RadioButton colors the ring (a components::Ring), shows the
	 * inner dot while selected and places the label each frame, in the
	 * Layout phase. The class itself holds no field: its setters write the
	 * components and restyle at once.
	 *
	 * @code
	 * RadioButton::Builder builder(getComponentRegistry(),
	 * 	getEntityRegistry());
	 * auto radio = builder.makeRadioButton(parent,
	 * 	RadioButton::Variant::Enabled, "Standard delivery");
	 * radio->setOnPress([] {
	 * 	// called when the user selects the radio button
	 * });
	 * @endcode
	 *
	 * `RadioButton::create(registry, config, variant, label)` builds the
	 * same widget without registering it in an entity registry; `config` is
	 * the SurfaceConfig of the ring (pose, size).
	 *
	 * A radio button alone does not deselect the others: a press only
	 * selects it. Use RadioGroup to keep one option selected among several.
	 * The radio button owns its dot and label children: it is the entity
	 * registry they are registered in. The label child is built only when
	 * the label given at creation is not empty. The ring is 20 dp wide and
	 * answers input on a TouchTarget square centered on it. Its Color and
	 * enabled state are written each frame from the selection and the
	 * variant, so values set by hand on them are overwritten. select() does
	 * not call the press handler; only a user press does.
	 *
	 * @see components::RadioButton, systems::RadioButton, RadioGroup
	 */
	class RadioButton:
		public SurfaceBase<components::Selectable, components::Ring,
						   components::RadioButton>
	{
		public:
		/// Visual variant: Enabled, Disabled (ignores input) or Error
		/// (error colors).
		using Variant = components::RadioButton::Variant;

		/**
		 * @brief Configures, creates and registers RadioButton widgets.
		 *
		 * Set options with the `with...` methods then call registerEntity(),
		 * or use makeRadioButton() to do both in one call.
		 */
		class Builder: public EntityBuilderBase<RadioButton>
		{
			private:
			SurfaceConfig _config;	  ///< Pose, layout and size of the ring.
			Variant _variant { Variant::Enabled };	  ///< Visual variant.
			std::string _label {};	  ///< Label text; empty for no label.

			public:
			/**
			 * @brief Construct a builder with the default configuration (see
			 * reset()).
			 * @param componentRegistry The registry that stores the
			 * components of the radio buttons to build.
			 * @param entityRegistry The registry the built radio buttons are
			 * added to (usually the scene's).
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a RadioButton, with its dot and label children,
			 * from the current configuration, without registering it.
			 *
			 * registerEntity() calls it, then sets the parent, registers the
			 * radio button and resets the builder.
			 * @return The new radio button, not selected.
			 */
			std::shared_ptr<RadioButton> buildEntity(void) override;

			/**
			 * @brief Restore the default configuration.
			 *
			 * Defaults: Variant::Enabled, no label, default pose. The ring
			 * is a 20 dp circle with no padding.
			 */
			void reset(void) override;

			/**
			 * @brief Set the position and orientation of the ring.
			 * @param pose The pose written to the radio button's Transform.
			 * Default: a default-constructed pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the visual variant.
			 * @param variant Enabled, Disabled (ignores input) or Error
			 * (error colors). Default: Variant::Enabled.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the label shown to the right of the ring.
			 * @param label The label text, also used as the accessibility
			 * label. Default: empty, no label child is built.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &label);

			/**
			 * @brief Build a radio button, register it under `parent` and
			 * reset the builder.
			 *
			 * A pose set before the call with withPose() also applies.
			 * @param parent The entity the radio button becomes a child of.
			 * @param variant Enabled, Disabled or Error.
			 * @param label The label text; empty for a radio button without
			 * label.
			 * @return The new radio button, not selected, registered in the
			 * entity registry.
			 */
			std::shared_ptr<RadioButton>
				makeRadioButton(std::shared_ptr<ecs::Entity> parent,
								Variant variant, const std::string &label);
		};

		public:
		/**
		 * @brief Create a radio button and its dot and label children,
		 * without registering it in an entity registry.
		 *
		 * The constructor is private because the children need a radio
		 * button already held by a `std::shared_ptr` as parent: create()
		 * constructs the radio button then calls build(), which creates
		 * them.
		 * @tparam Arguments The types of the constructor arguments.
		 * @param arguments The arguments forwarded to the constructor:
		 * (registry, config, variant, label).
		 * @return The new radio button, not selected.
		 */
		template<typename... Arguments>
		static std::shared_ptr<RadioButton> create(Arguments &&...arguments)
		{
			std::shared_ptr<RadioButton> entity(
				new RadioButton(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~RadioButton(void) override = default;

		/**
		 * @brief Set the visual variant. Restyles at once.
		 * @param variant Enabled, Disabled (ignores input) or Error (error
		 * colors).
		 * @return Reference to this radio button for chaining.
		 */
		RadioButton &setVariant(Variant variant);

		/**
		 * @brief Get the visual variant.
		 * @return The variant stored in components::RadioButton.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the label text and the accessibility label. Restyles at
		 * once.
		 *
		 * The text on screen changes only when the radio button was created
		 * with a label: no label child is built here.
		 * @param label The new label text.
		 * @return Reference to this radio button for chaining.
		 */
		RadioButton &setLabel(const std::string &label);

		/**
		 * @brief Check whether the radio button is selected.
		 * @return True when its components::Selectable is selected. A new
		 * radio button is not selected.
		 */
		bool isSelected(void) const;

		/**
		 * @brief Select or deselect the radio button, even when it is
		 * disabled. Restyles at once.
		 *
		 * The press handler is not called and no other radio button is
		 * changed.
		 * @param selected True to select, false to deselect.
		 * @return Reference to this radio button for chaining.
		 */
		RadioButton &select(bool selected);

		/**
		 * @brief Check whether the radio button ignores input.
		 * @return True for `Variant::Disabled`.
		 */
		bool isDisabled(void) const;

		/**
		 * @brief Set the handler called when the user presses the radio
		 * button (Paper `onPress`).
		 *
		 * The press selects the radio button first, then calls the handler,
		 * also when the radio button was already selected. A disabled radio
		 * button calls nothing.
		 * @param onPress The function to call. Pass an empty function to
		 * remove the handler.
		 * @return Reference to this radio button for chaining.
		 */
		RadioButton &setOnPress(std::function<void(void)> onPress);

		/**
		 * @brief Diameter of the dot shown while selected, in dp (M3:
		 * 10 dp).
		 */
		static constexpr float DotSize = components::RadioButton::DotSize;

		/**
		 * @brief Side of the square that answers input, centered on the
		 * ring, in dp.
		 */
		static constexpr float TouchTarget =
			components::RadioButton::TouchTarget;

		/**
		 * @brief Gap between the touch target and the label, in dp.
		 */
		static constexpr float LabelGap = components::RadioButton::LabelGap;

		private:
		/**
		 * @brief Construct a radio button without its children; create()
		 * calls it then build().
		 *
		 * Fills components::RadioButton and installs systems::RadioButton
		 * on the entity.
		 * @param registry The registry that stores the radio button's
		 * components.
		 * @param config The pose, layout and size of the ring.
		 * @param variant Enabled, Disabled or Error.
		 * @param label The label text; empty for no label.
		 */
		RadioButton(ecs::ComponentRegistry &registry,
					const SurfaceConfig &config, Variant variant,
					const std::string &label);

		/**
		 * @brief Create the dot child and, when the label is not empty, the
		 * label child, and record their identifiers in
		 * components::RadioButton.
		 *
		 * Called once by create(), when the radio button is held by a
		 * `std::shared_ptr` and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
