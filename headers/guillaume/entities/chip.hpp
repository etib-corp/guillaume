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
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/elevation.hpp"
#include "guillaume/components/selection.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Chip entity: a compact pill-shaped action or filter.
	 *
	 * The chip reuses `SurfaceBase` for the pill geometry and
	 * `components::Selectable` for the selected state. The Material Design
	 * variants are:
	 * - `Assist`: leading action icon plus a label.
	 * - `Filter`: leading check icon toggling a secondary-container fill.
	 * - `Input`: label plus a trailing remove icon.
	 * - `Suggestion`: label only.
	 *
	 * @see SurfaceBase
	 * @see components::Selectable
	 */
	class Chip:
		public SurfaceBase<components::Selectable, components::Elevation>
	{
		public:
		/**
		 * @brief Chip variant.
		 */
		enum class Variant { Assist, Filter, Input, Suggestion };

		/**
		 * @brief Paper chip mode: tonal flat (default) or outlined.
		 */
		enum class Mode { Flat, Outlined };

		/**
		 * @brief Builder used to configure and create `Chip` entities.
		 */
		class Builder: public EntityBuilderBase<Chip>
		{
			private:
			SurfaceConfig _config;					 ///< Surface configuration.
			Variant _variant { Variant::Assist };	 ///< Chip variant.
			std::string _label {};					 ///< Chip label.
			std::string _iconGlyph {};				 ///< Leading icon glyph.
			bool _selected { false };				 ///< Selected state.

			public:
			/**
			 * @brief Construct a new Chip Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Chip entity from the current configuration.
			 * @return The newly created Chip.
			 */
			std::shared_ptr<Chip> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the chip pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the chip variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the chip label.
			 * @param label The new label.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withLabel(const std::string &label);

			/**
			 * @brief Set the leading icon glyph.
			 * @param iconGlyph The new icon glyph.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withIcon(const std::string &iconGlyph);

			/**
			 * @brief Set the selected state.
			 * @param selected The new selected state.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSelected(bool selected);
		};

		/**
		 * @brief Director that orchestrates `Chip::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create a chip using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The chip variant.
			 * @param label The chip label.
			 * @param iconGlyph The leading icon glyph.
			 * @param selected The selected state.
			 * @return The newly created chip.
			 */
			std::shared_ptr<Chip>
				makeChip(Builder &builder, std::shared_ptr<ecs::Entity> parent,
						 Variant variant, const std::string &label,
						 const std::string &iconGlyph, bool selected);
		};

		private:
		Variant _variant { Variant::Assist };	 ///< Chip variant.
		std::string _label {};					 ///< Chip label.
		std::string _iconGlyph {};				 ///< Leading icon glyph.
		std::shared_ptr<Icon> _leadingIcon;		 ///< Leading icon child.
		std::shared_ptr<Icon> _trailingIcon;	 ///< Trailing icon child.
		std::shared_ptr<Text> _labelEntity;		 ///< Label child.
		std::shared_ptr<Icon> _checkIcon;		 ///< Selected check child.
		Mode _mode { Mode::Flat };				 ///< Chip mode.
		bool _compact { false };				 ///< Compact padding.
		bool _elevated { false };				 ///< Level-1 shadow.
		bool _showSelectedOverlay { false };	 ///< Selected state layer.
		bool _showSelectedCheck { true };		 ///< Check when selected.
		std::function<void(void)> _onPress {};	 ///< Activation handler.
		std::function<void(void)> _onClose {};	 ///< Close-icon handler.

		private:
		void applyVariant(void);

		/**
		 * @brief Rebuild the layout children (check shown only when selected).
		 */
		void attachChildren(void);

		/**
		 * @brief Handle an activation: toggle filter chips, then notify.
		 */
		void pressHandler(void);

		public:
		/**
		 * @brief Construct a Chip entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The chip variant.
		 * @param label The chip label.
		 * @param iconGlyph The leading icon glyph.
		 * @param selected The selected state.
		 */
		Chip(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			 Variant variant, const std::string &label,
			 const std::string &iconGlyph, bool selected);

		/**
		 * @brief Default destructor.
		 */
		~Chip(void) override;

		/**
		 * @brief Set the chip variant.
		 * @param variant The new variant.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setVariant(Variant variant);

		/**
		 * @brief Get the chip variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the chip label.
		 * @param label The new label.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setLabel(const std::string &label);

		/**
		 * @brief Set the selected state.
		 * @param selected The new selected state.
		 * @return Reference to this chip for chaining.
		 */
		Chip &setSelected(bool selected);

		/**
		 * @brief Whether the chip is currently selected.
		 * @return True when selected.
		 */
		bool isSelected(void) const;

		/** @brief Set the mode. @param mode New mode. @return *this. */
		Chip &setMode(Mode mode);

		/** @brief Get the mode. @return The mode. */
		Mode getMode(void) const;

		/**
		 * @brief Enable or disable the chip.
		 * @param disabled True to disable.
		 * @return *this.
		 */
		Chip &setDisabled(bool disabled);

		/** @brief Check whether disabled. @return True when disabled. */
		bool isDisabled(void) const;

		/**
		 * @brief Use compact padding (Paper `compact`).
		 * @param compact New flag.
		 * @return *this.
		 */
		Chip &setCompact(bool compact);

		/**
		 * @brief Paint a level-1 shadow (Paper `elevated`).
		 * @param elevated New flag.
		 * @return *this.
		 */
		Chip &setElevated(bool elevated);

		/**
		 * @brief Overlay a pressed state layer while selected (Paper
		 * `showSelectedOverlay`).
		 * @param show New flag.
		 * @return *this.
		 */
		Chip &setShowSelectedOverlay(bool show);

		/**
		 * @brief Show the check glyph while selected (Paper
		 * `showSelectedCheck`).
		 * @param show New flag.
		 * @return *this.
		 */
		Chip &setShowSelectedCheck(bool show);

		/**
		 * @brief Set the activation handler (Paper `onPress`).
		 * @param onPress Handler.
		 * @return *this.
		 */
		Chip &setOnPress(std::function<void(void)> onPress);

		/**
		 * @brief Set the close-icon handler (Paper `onClose`); the trailing
		 * close icon is pressable.
		 * @param onClose Handler.
		 * @return *this.
		 */
		Chip &setOnClose(std::function<void(void)> onClose);

		/**
		 * @brief Get the trailing close icon identifier.
		 * @return The identifier, or InvalidIdentifier.
		 */
		ecs::Entity::Identifier getCloseIconIdentifier(void) const;

		/**
		 * @brief Initialize the chip derived state and children.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the chip derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
