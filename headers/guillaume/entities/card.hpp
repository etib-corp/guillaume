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
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/animation.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{
	class Button;
	class Image;
	class Text;

	/**
	 * @brief Material 3 card (react-native-paper `Card`).
	 *
	 * Paper's modes map to `Variant::Elevated` (default, `elevation.level1`
	 * background, level 1 shadow raised to level 2 while pressed),
	 * `Variant::Filled` (Paper `contained`, `surfaceVariant`) and
	 * `Variant::Outlined` (`surface` with an `outlineVariant` stroke).
	 *
	 * Content is composed like Paper's `Card.Cover`, `Card.Title`,
	 * `Card.Content` and `Card.Actions`, always laid out in that order.
	 * Setting a press handler makes the card interactive (hover/press state
	 * layers, focus, Button role); a disabled card ignores presses.
	 */
	class Card: public SurfaceBase<components::Elevation, components::Animation>
	{
		public:
		/**
		 * @brief Card variants.
		 */
		enum class Variant {
			Elevated,	 ///< Tonal level-1 surface with shadow.
			Filled,		 ///< Paper `contained`: surfaceVariant.
			Outlined	 ///< Surface with an outlineVariant stroke.
		};

		/**
		 * @brief Height of `Card.Cover` (Paper: 195 dp).
		 */
		static constexpr float CoverHeight = 195.0f;

		/**
		 * @brief Minimum height of `Card.Title` (Paper: 72 dp).
		 */
		static constexpr float TitleMinHeight = 72.0f;

		/**
		 * @brief Builder for Card entities.
		 */
		class Builder: public EntityBuilderBase<Card>
		{
			private:
			SurfaceConfig _config;	  ///< Surface configuration.
			Variant _variant { Variant::Elevated };	   ///< Card variant.
			std::function<void(void)> _onPress {};	   ///< Press handler.
			bool _disabled { false };				   ///< Disabled flag.

			public:
			/**
			 * @brief Construct a card builder.
			 * @param componentRegistry Component registry.
			 * @param entityRegistry Entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the card.
			 * @return The card.
			 */
			std::shared_ptr<Card> buildEntity(void) override;

			/**
			 * @brief Reset to the defaults.
			 */
			void reset(void) override;

			/** @brief Set the pose. @param pose Pose. @return *this. */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/** @brief Set the variant. @param variant Variant. @return *this.
			 */
			Builder &withVariant(Variant variant);

			/** @brief Set a fixed width. @param width Width. @return *this. */
			Builder &withWidth(float width);

			/** @brief Set a fixed height. @param height Height. @return *this.
			 */
			Builder &withHeight(float height);

			/**
			 * @brief Set the corner radius.
			 * @param radius Radius.
			 * @return *this.
			 */
			Builder &withBorderRadius(float radius);

			/**
			 * @brief Make the card pressable.
			 * @param onPress Press handler.
			 * @return *this.
			 */
			Builder &withOnPress(std::function<void(void)> onPress);

			/**
			 * @brief Set the disabled state.
			 * @param disabled Disabled flag.
			 * @return *this.
			 */
			Builder &withDisabled(bool disabled);

			/**
			 * @brief Build a card.
			 * @param parent Parent entity.
			 * @param variant Variant.
			 * @return The card.
			 */
			std::shared_ptr<Card> makeCard(std::shared_ptr<ecs::Entity> parent,
										   Variant variant);
		};

		/**
		 * @brief Construct a card.
		 * @param registry Component registry.
		 * @param config Surface configuration.
		 * @param variant Variant.
		 */
		Card(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			 Variant variant);

		/**
		 * @brief Destructor.
		 */
		~Card(void) override;

		/** @brief Set the variant. @param variant Variant. @return *this. */
		Card &setVariant(Variant variant);

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const;

		/**
		 * @brief Make the card pressable (Paper `onPress`).
		 * @param onPress Press handler; an empty handler keeps the card
		 * interactive but does nothing on press.
		 * @return *this.
		 */
		Card &setOnPress(std::function<void(void)> onPress);

		/**
		 * @brief Check whether the card reacts to presses.
		 * @return True once a press handler was set.
		 */
		bool isInteractive(void) const;

		/**
		 * @brief Set the disabled state (Paper `disabled`).
		 * @param disabled Disabled flag.
		 * @return *this.
		 */
		Card &setDisabled(bool disabled);

		/** @brief Check the disabled state. @return True when disabled. */
		bool isDisabled(void) const;

		/**
		 * @brief Compose a `Card.Cover` image above the other sections.
		 * @param texturePath Cover texture.
		 * @return *this.
		 */
		Card &setCover(const std::string &texturePath);

		/**
		 * @brief Compose a `Card.Title` section.
		 * @param title Title (bodyLarge).
		 * @param subtitle Optional subtitle (bodyMedium).
		 * @return *this.
		 */
		Card &setTitle(const std::string &title,
					   const std::string &subtitle = "");

		/**
		 * @brief Compose a `Card.Content` text.
		 * @param content Content text (bodyMedium).
		 * @return *this.
		 */
		Card &setContent(const std::string &content);

		/**
		 * @brief Append a text button to the `Card.Actions` row.
		 * @param label Action label.
		 * @param onPress Action handler.
		 * @return *this.
		 */
		Card &addAction(const std::string &label,
						std::function<void(void)> onPress);

		/** @brief Get the cover entity. @return The cover, or nullptr. */
		std::shared_ptr<Image> getCover(void) const;

		/** @brief Get the title section. @return The section, or nullptr. */
		std::shared_ptr<ecs::Entity> getTitleSection(void) const;

		/** @brief Get the content text. @return The text, or nullptr. */
		std::shared_ptr<Text> getContent(void) const;

		/** @brief Get the actions row. @return The row, or nullptr. */
		std::shared_ptr<ecs::Entity> getActionsSection(void) const;

		/** @brief Get the action buttons. @return The buttons. */
		const std::vector<std::shared_ptr<Button>> &getActions(void) const;

		/**
		 * @brief Get the press-elevation progress (0 at rest, 1 pressed).
		 * @return The progress in [0, 1].
		 */
		float getPressProgress(void) const;

		/**
		 * @brief Build the composed sections.
		 */
		void initialize(void) override;

		/**
		 * @brief Restyle the card.
		 */
		void update(void) override;

		private:
		/**
		 * @brief Composition data of one action.
		 */
		struct ActionSpec {
			std::string label;					  ///< Button label.
			std::function<void(void)> onPress;	  ///< Button handler.
		};

		Variant _variant { Variant::Elevated };	   ///< Card variant.
		bool _interactive { false };			   ///< Whether pressable.
		std::function<void(void)> _onPress {};	   ///< Press handler.
		bool _pressed { false };				   ///< Last pressed state.
		float _pressProgress { 0.0f };			   ///< Elevation progress.
		float _pressFrom { 0.0f };		///< Progress when the motion began.
		bool _initialized { false };	///< Whether children may be built.
		std::string _coverPath {};		///< Cover texture.
		std::string _title {};			///< Title text.
		std::string _subtitle {};		///< Subtitle text.
		std::string _contentText {};	///< Content text.
		std::vector<ActionSpec> _actionSpecs {};		 ///< Requested actions.
		std::shared_ptr<Image> _cover;					 ///< Cover image.
		std::shared_ptr<ecs::Entity> _titleSection;		 ///< Title section.
		std::shared_ptr<Text> _titleText;				 ///< Title text.
		std::shared_ptr<Text> _subtitleText;			 ///< Subtitle text.
		std::shared_ptr<Text> _content;					 ///< Content text.
		std::shared_ptr<ecs::Entity> _actionsSection;	 ///< Actions row.
		std::vector<std::shared_ptr<Button>> _actions;	  ///< Action buttons.

		/**
		 * @brief Apply colors, stroke and elevation for the current state.
		 */
		void applyVariant(void);

		/**
		 * @brief React to a hover/press change from the interaction model.
		 */
		void onInteractionChanged(void);

		/**
		 * @brief Create the missing composed sections and order them.
		 */
		void buildComposition(void);
	};

}	 // namespace guillaume::entities
