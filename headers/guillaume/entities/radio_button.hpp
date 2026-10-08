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

#include <memory>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/ring.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Radio button entity: a single-choice circle with an optional
	 * label.
	 *
	 * The radio reuses `SurfaceBase` for its geometry and
	 * `components::Selectable` for the selected state. The outer circle is a
	 * `components::Ring` (outline when unselected, primary when selected) and
	 * a small filled inner dot is shown when selected.
	 *
	 * @see SurfaceBase
	 * @see components::Selectable
	 * @see components::Ring
	 */
	class RadioButton:
		public SurfaceBase<components::Selectable, components::Ring>
	{
		public:
		/**
		 * @brief Radio button visual variant.
		 */
		enum class Variant { Enabled, Disabled, Error };

		/**
		 * @brief Builder used to configure and create `RadioButton` entities.
		 */
		class Builder: public EntityBuilderBase<RadioButton>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Enabled };	  ///< Visual variant.
			std::string _label {};					  ///< Label content.

			public:
			/**
			 * @brief Construct a new RadioButton Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the RadioButton entity from the current
			 * configuration.
			 * @return The newly created RadioButton.
			 */
			std::shared_ptr<RadioButton> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the radio button pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the radio button variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the radio button label.
			 * @param label The new label.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &label);
		};

		/**
		 * @brief Director that orchestrates `RadioButton::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a radio button using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The radio button variant.
			 * @param label The radio button label.
			 * @return The newly created radio button.
			 */
			std::shared_ptr<RadioButton>
				makeRadioButton(Builder &builder,
								std::shared_ptr<ecs::Entity> parent,
								Variant variant, const std::string &label);
		};

		private:
		Variant _variant { Variant::Enabled };	  ///< Visual variant.
		std::string _label {};					  ///< Label content.
		std::shared_ptr<ecs::Entity> _dot;		  ///< Inner filled dot child.
		std::shared_ptr<Text> _labelEntity;		  ///< Label child.

		private:
		/**
		 * @brief Apply the variant-specific ring and dot colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Refresh the inner dot and label styling from the selection
		 * state.
		 */
		void applyState(void);

		public:
		/**
		 * @brief Construct a RadioButton entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The visual variant.
		 * @param label The label content.
		 */
		RadioButton(ecs::ComponentRegistry &registry,
					const SurfaceConfig &config, Variant variant,
					const std::string &label);

		/**
		 * @brief Default destructor.
		 */
		~RadioButton(void) override;

		/**
		 * @brief Set the radio button variant.
		 * @param variant The new variant.
		 * @return Reference to this radio button for chaining.
		 */
		RadioButton &setVariant(Variant variant);

		/**
		 * @brief Get the radio button variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the radio button label.
		 * @param label The new label.
		 * @return Reference to this radio button for chaining.
		 */
		RadioButton &setLabel(const std::string &label);

		/**
		 * @brief Whether the radio button is currently selected.
		 * @return True when selected.
		 */
		bool isSelected(void) const;

		/**
		 * @brief Select or deselect the radio button.
		 * @param selected The new selected state.
		 * @return Reference to this radio button for chaining.
		 */
		RadioButton &select(bool selected);

		/**
		 * @brief Initialize the radio button derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the radio button derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
