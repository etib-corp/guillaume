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
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/animation.hpp"
#include "guillaume/components/card.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"

namespace guillaume::entities
{
	class Button;
	class Image;
	class Text;

	/**
	 * @brief A rounded panel grouping a cover image, a title, a text and
	 * action buttons about one subject (react-native-paper `Card`).
	 *
	 * Its state lives in components::Card (variant, press handler, texts of
	 * the sections and identifiers of their entities); systems::Card styles
	 * the panel, runs its press elevation and arranges its sections each
	 * frame, in the Layout phase. The class itself holds no field: its
	 * setters write the component, create the child entities a section
	 * needs, and restyle at once.
	 *
	 * The variant chooses the look:
	 * - `Elevated` (default): tinted surface with a level 1 shadow, raised
	 *   to level 2 while pressed.
	 * - `Filled`: SurfaceVariant color, no shadow (Paper `contained`).
	 * - `Outlined`: Surface color with a 1 dp OutlineVariant stroke.
	 *
	 * @code
	 * Card::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto card = builder.withWidth(320.0f).makeCard(parent,
	 * 	Card::Variant::Elevated);
	 * card->setCover("images/mountain.png");
	 * card->setTitle("Mont Blanc", "4806 m");
	 * card->setContent("The highest summit of the Alps.");
	 * card->addAction("Share", [] {
	 * 	// called when the user presses the button
	 * });
	 * @endcode
	 *
	 * `Card::create(registry, config, variant)` builds the same widget
	 * without registering it in an entity registry.
	 *
	 * Watch out:
	 * - The sections are always laid out in the order cover, title,
	 *   content, actions, whatever the order of the calls. Once one exists,
	 *   the sections replace any layout children given with setChildren().
	 * - A section takes the inner width of the card (fixed width minus
	 *   padding) when it is first created. Set the width with the builder
	 *   before composing; a card without a fixed width gives its cover no
	 *   size.
	 * - The first call that gives a section a non-empty value creates its
	 *   entities; later calls only change their text. A subtitle shows
	 *   only if the first non-empty title came with one.
	 * - The card owns its cover, title section, content text and actions
	 *   row; the actions row owns the buttons.
	 * - A card reacts to input only after setOnPress(), and cannot be made
	 *   passive again.
	 * - systems::Card overwrites the fill color, the stroke and the
	 *   elevation every frame; SurfaceBase::setColor() does not last. The
	 *   corner radius is kept.
	 *
	 * @see components::Card, systems::Card, SurfaceBase
	 */
	class Card:
		public SurfaceBase<components::Elevation, components::Animation,
						   components::Card>
	{
		public:
		/// Look of the card: `Elevated`, `Filled` or `Outlined`.
		using Variant = components::Card::Variant;

		/**
		 * @brief Height given to the cover image, in dp (Paper `Card.Cover`:
		 * 195).
		 */
		static constexpr float CoverHeight = components::Card::CoverHeight;

		/**
		 * @brief Height given to the title section, in dp (the minimum
		 * height of Paper `Card.Title`: 72).
		 */
		static constexpr float TitleMinHeight =
			components::Card::TitleMinHeight;

		/**
		 * @brief Configures Card widgets and registers them in the scene's
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<Card>
		{
			private:
			SurfaceConfig _config;	  ///< Pose, size, radius and layout.
			Variant _variant { Variant::Elevated };	   ///< Look of the card.
			std::function<void(void)>
				_onPress {};	///< Press handler (empty: passive card).
			bool _disabled { false };	 ///< Whether presses are ignored.

			public:
			/**
			 * @brief Construct a builder holding the defaults of reset().
			 * @param componentRegistry The registry the components of the
			 * built cards are stored in.
			 * @param entityRegistry The registry that owns the cards built
			 * by `makeCard`.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build an empty card from the current configuration,
			 * without registering it or giving it a parent.
			 * @return The new card.
			 */
			std::shared_ptr<Card> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: default pose, the `Elevated`
			 * variant, no press handler, enabled, the Medium shape radius
			 * of the active theme, a fixed width of 240 dp, no fixed
			 * height, and a vertical layout with 16 dp of padding and 8 dp
			 * between sections.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the card is.
			 * @param pose The pose of the top-left corner of the card. A
			 * default-constructed pose by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Choose the look of the card.
			 * @param variant `Elevated`, `Filled` or `Outlined`.
			 * `Elevated` by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the width of the card.
			 * @param width The fixed width, in dp. 240 by default. The
			 * sections are sized from it.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withWidth(float width);

			/**
			 * @brief Force the height of the card.
			 * @param height The fixed height, in dp. By default the height
			 * follows the sections.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withHeight(float height);

			/**
			 * @brief Set the corner radius.
			 * @param radius The radius of the four corners, in dp. By
			 * default the Medium shape radius the active theme had when the
			 * builder was last reset.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withBorderRadius(float radius);

			/**
			 * @brief Make the built card pressable.
			 * @param onPress The function called when the user presses the
			 * card. An empty function, the default, builds a passive card.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withOnPress(std::function<void(void)> onPress);

			/**
			 * @brief Build the card disabled or enabled.
			 * @param disabled True for a card that ignores presses. False
			 * by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withDisabled(bool disabled);

			/**
			 * @brief Build an empty card, register it under `parent` and
			 * reset the builder.
			 *
			 * The pose, size, radius, press handler and disabled state set
			 * with the `with...` methods apply. Compose the content on the
			 * returned card with setCover(), setTitle(), setContent() and
			 * addAction().
			 * @param parent The entity the card is attached to, or nullptr
			 * for a root entity.
			 * @param variant The look: `Elevated`, `Filled` or `Outlined`.
			 * @return The registered card.
			 */
			std::shared_ptr<Card> makeCard(std::shared_ptr<ecs::Entity> parent,
										   Variant variant);
		};

		/**
		 * @brief Build a card without registering it in an entity registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * `(registry, config, variant)`. It then calls build(). A new card
		 * has no section yet, so no child is created at this point: the
		 * sections are built by setCover(), setTitle(), setContent() and
		 * addAction().
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new card. The caller owns it.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Card> create(Arguments &&...arguments)
		{
			std::shared_ptr<Card> entity(
				new Card(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~Card(void) override;

		/**
		 * @brief Change the look of the card. Restyles at once.
		 * @param variant `Elevated`, `Filled` or `Outlined`.
		 * @return Reference to this card for chaining.
		 */
		Card &setVariant(Variant variant);

		/**
		 * @brief Get the look of the card.
		 * @return The variant stored in components::Card.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Make the card pressable and set what a press does (Paper
		 * `onPress`).
		 *
		 * The first call adds the interaction components to the card: hover
		 * and press state layers, focus and the Button accessibility role.
		 * Later calls only replace the handler.
		 * @param onPress The function called when the user presses the
		 * card. An empty function keeps the card interactive but does
		 * nothing on press.
		 * @return Reference to this card for chaining.
		 */
		Card &setOnPress(std::function<void(void)> onPress);

		/**
		 * @brief Check whether the card reacts to input.
		 * @return True once setOnPress() was called, even with an empty
		 * function, and whether or not the card is disabled.
		 */
		bool isInteractive(void) const;

		/**
		 * @brief Disable or enable the card (Paper `disabled`). Restyles at
		 * once.
		 *
		 * A disabled card does not call its press handler. Disabling a
		 * pressed card drops its elevation back to rest.
		 * @param disabled True to disable the card, false to enable it.
		 * @return Reference to this card for chaining.
		 */
		Card &setDisabled(bool disabled);

		/**
		 * @brief Check whether the card ignores presses.
		 * @return True after setDisabled(true). A new card is enabled.
		 */
		bool isDisabled(void) const;

		/**
		 * @brief Show an image at the top of the card (Paper `Card.Cover`).
		 *
		 * The first call with a non-empty path creates the Image child,
		 * `CoverHeight` high and as wide as the inside of the card. Later
		 * calls change its texture.
		 * @param texturePath The path of the texture file to draw.
		 * @return Reference to this card for chaining.
		 */
		Card &setCover(const std::string &texturePath);

		/**
		 * @brief Show a title, and optionally a subtitle under it (Paper
		 * `Card.Title`).
		 *
		 * The first call with a non-empty title creates the title section
		 * (`TitleMinHeight` high) and its Text children; the subtitle child
		 * is created only when @p subtitle is not empty in that call. Later
		 * calls change the text of the existing children.
		 * @param title The title, drawn with the BodyLarge role. It is also
		 * the accessibility label of the section.
		 * @param subtitle The subtitle, drawn with the BodyMedium role.
		 * Empty by default.
		 * @return Reference to this card for chaining.
		 */
		Card &setTitle(const std::string &title,
					   const std::string &subtitle = "");

		/**
		 * @brief Show a text under the title (Paper `Card.Content`).
		 *
		 * The first call with a non-empty text creates the Text child.
		 * Later calls change its content.
		 * @param content The text, drawn on one line with the BodyMedium
		 * role.
		 * @return Reference to this card for chaining.
		 */
		Card &setContent(const std::string &content);

		/**
		 * @brief Append a text button to the row at the bottom of the card
		 * (Paper `Card.Actions`).
		 *
		 * The first call creates the row, which aligns its buttons to the
		 * end. Each call builds one Button with the `Button::Color::Text`
		 * style. An action cannot be removed.
		 * @param label The label of the button.
		 * @param onPress The function called when the user presses the
		 * button.
		 * @return Reference to this card for chaining.
		 */
		Card &addAction(const std::string &label,
						std::function<void(void)> onPress);

		/**
		 * @brief Get the cover image.
		 * @return The Image child, or nullptr before setCover() created it.
		 */
		std::shared_ptr<Image> getCover(void) const;

		/**
		 * @brief Get the section holding the title and subtitle texts.
		 * @return The section entity, or nullptr before setTitle() created
		 * it.
		 */
		std::shared_ptr<ecs::Entity> getTitleSection(void) const;

		/**
		 * @brief Get the content text.
		 * @return The Text child, or nullptr before setContent() created
		 * it.
		 */
		std::shared_ptr<Text> getContent(void) const;

		/**
		 * @brief Get the row holding the action buttons.
		 * @return The row entity, or nullptr before the first addAction().
		 */
		std::shared_ptr<ecs::Entity> getActionsSection(void) const;

		/**
		 * @brief Get the action buttons.
		 *
		 * The card keeps no list of them: the buttons are resolved from the
		 * identifiers recorded in components::Card on every call.
		 * @return The buttons, in the order they were added. Empty when
		 * the card has no action.
		 */
		std::vector<std::shared_ptr<Button>> getActions(void) const;

		/**
		 * @brief Get how far the press elevation has progressed.
		 *
		 * Only an `Elevated` card animates it, over 150 ms.
		 * @return The progress, from 0 (at rest, level 1) to 1 (pressed,
		 * level 2).
		 */
		float getPressProgress(void) const;

		private:
		/**
		 * @brief Create the missing composed sections, record their
		 * identifiers in components::Card and order them.
		 *
		 * Does nothing until the card is owned by a shared pointer.
		 */
		void buildComposition(void);

		private:
		/**
		 * @brief Construct a card, set up its press animation and style
		 * it.
		 * @param registry The registry the components are stored in.
		 * @param config The pose, fixed size, corner radius and layout of
		 * the card. Its color is replaced by systems::Card.
		 * @param variant The look: `Elevated`, `Filled` or `Outlined`.
		 */
		Card(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			 Variant variant);

		/**
		 * @brief Create the entities of the sections already described in
		 * components::Card, then style the card.
		 *
		 * Called once by create(), when the card is held by a shared
		 * pointer and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
