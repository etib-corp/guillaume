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

#include "guillaume/components/hand_button_interaction.hpp"
#include "guillaume/components/mouse_button_interaction.hpp"
#include "guillaume/components/value.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Switch entity: a toggle track with a sliding thumb.
	 *
	 * The switch reuses `SurfaceBase` for the track geometry and
	 * `components::Value` (0/1) as the on/off state. The thumb is a child
	 * ellipse positioned at the left or right end of the track depending on
	 * the value.
	 *
	 * @see SurfaceBase
	 * @see components::Value
	 */
	class Switch:
		public SurfaceBase<components::Value,
						   components::MouseButtonInteraction,
						   components::HandButtonInteraction>
	{
		public:
		/**
		 * @brief Switch visual variant.
		 */
		enum class Variant { Enabled, Disabled, WithIcon };

		/**
		 * @brief Builder used to configure and create `Switch` entities.
		 */
		class Builder: public EntityBuilderBase<Switch>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Enabled };	  ///< Visual variant.
			bool _checked { false };	  ///< Initial checked state.
			std::string _iconGlyph {};	  ///< Thumb icon glyph.
			std::string _label {};		  ///< Label content.

			public:
			/**
			 * @brief Construct a new Switch Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Switch entity from the current configuration.
			 * @return The newly created Switch.
			 */
			std::shared_ptr<Switch> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the switch pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the switch variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the initial checked state.
			 * @param checked The new checked state.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withChecked(bool checked);

			/**
			 * @brief Set the thumb icon glyph.
			 * @param iconGlyph The new icon glyph.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the switch label.
			 * @param label The new label.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &label);
		};

		/**
		 * @brief Director that orchestrates `Switch::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a switch using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The switch variant.
			 * @param checked The initial checked state.
			 * @param iconGlyph The thumb icon glyph.
			 * @param label The switch label.
			 * @return The newly created switch.
			 */
			std::shared_ptr<Switch>
				makeSwitch(Builder &builder,
						   std::shared_ptr<ecs::Entity> parent, Variant variant,
						   bool checked, const std::string &iconGlyph,
						   const std::string &label);
		};

		private:
		Variant _variant { Variant::Enabled };	  ///< Visual variant.
		std::string _iconGlyph {};				  ///< Thumb icon glyph.
		std::string _label {};					  ///< Label content.
		std::shared_ptr<ecs::Entity> _thumb;	  ///< Thumb child.
		std::shared_ptr<Icon> _icon;			  ///< Thumb icon child.
		std::shared_ptr<Text> _labelEntity;		  ///< Label child.

		private:
		/**
		 * @brief Apply the variant-specific colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Reposition the thumb and restyle it from the current value.
		 */
		void applyState(void);

		public:
		/**
		 * @brief Construct a Switch entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The visual variant.
		 * @param checked The initial checked state.
		 * @param iconGlyph The thumb icon glyph.
		 * @param label The label content.
		 */
		Switch(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, bool checked, const std::string &iconGlyph,
			   const std::string &label);

		/**
		 * @brief Default destructor.
		 */
		~Switch(void) override;

		/**
		 * @brief Set the switch variant.
		 * @param variant The new variant.
		 * @return Reference to this switch for chaining.
		 */
		Switch &setVariant(Variant variant);

		/**
		 * @brief Get the switch variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the switch checked state.
		 * @param checked The new checked state.
		 * @return Reference to this switch for chaining.
		 */
		Switch &setChecked(bool checked);

		/**
		 * @brief Whether the switch is currently checked.
		 * @return True when checked.
		 */
		bool isChecked(void) const;

		/**
		 * @brief Flip the switch state.
		 * @return Reference to this switch for chaining.
		 */
		Switch &toggle(void);

		/**
		 * @brief Set the switch label.
		 * @param label The new label.
		 * @return Reference to this switch for chaining.
		 */
		Switch &setLabel(const std::string &label);

		/**
		 * @brief Initialize the switch derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the switch derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
