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

#include <chrono>
#include <string>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/button.hpp"
#include "guillaume/theme/typescale.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Input and content rules shared by every button variant.
	 *
	 * Turns hover, press and release into components::Button state (press
	 * timing, toggle selection, click and long-press callbacks) and keeps the
	 * icon child's glyph in sync with it. Colors and geometry belong to the
	 * variant systems, reached through the button's restyle function. Runs in
	 * the Event phase.
	 */
	class Button: public ecs::SystemFiller<components::Button>
	{
		public:
		/**
		 * @brief View over a button child (icon or label) by identifier.
		 *
		 * Converts to false when the child does not exist, so variant systems
		 * can write `if (icon) { icon.setColor(...); }`.
		 */
		class Child
		{
			private:
			ecs::ComponentRegistry &_registry;		///< Component storage
			ecs::Entity::Identifier _identifier;	///< Child identifier

			public:
			/**
			 * @brief Construct a view.
			 * @param registry The registry holding the components.
			 * @param identifier The child identifier (may be invalid).
			 */
			Child(ecs::ComponentRegistry &registry,
				  ecs::Entity::Identifier identifier);

			/**
			 * @brief Whether the child exists.
			 * @return True when it has a Transform in the registry.
			 */
			explicit operator bool(void) const;

			/** @brief Get the child identifier. @return The identifier. */
			ecs::Entity::Identifier getIdentifier(void) const noexcept;

			/** @brief Set the Color. @param color It. @return The view. */
			const Child &
				setColor(const utility::graphic::Color32Bit &color) const;

			/** @brief Set the Glyph font size. @param size It. @return The
			 * view. */
			const Child &setFontSize(float size) const;

			/** @brief Set the Typography role. @param role It. @return The
			 * view. */
			const Child &setTypeRole(theme::TypeRole role) const;

			/** @brief Set the Text content. @param content It. @return The
			 * view. */
			const Child &setContent(const std::string &content) const;
		};

		/// Glyph shown in place of the icon while loading.
		static constexpr const char *LoadingGlyph = "progress_activity";

		/// Press duration after which a release is a long press.
		static constexpr std::chrono::milliseconds LongPressDuration { 500 };

		/**
		 * @brief Default constructor.
		 */
		Button(void);

		/**
		 * @brief Default destructor.
		 */
		~Button(void) override = default;

		/**
		 * @brief Wire hover, press and release of an entity to this system.
		 *
		 * The entity must already have its components::Button and the hand
		 * and mouse hover and button interaction components.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void install(ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &identifier);

		/**
		 * @brief React to a hover change: restyle unless disabled.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void hoverChanged(ecs::ComponentRegistry &registry,
								 const ecs::Entity::Identifier &identifier);

		/**
		 * @brief React to a press: start timing it and restyle.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void press(ecs::ComponentRegistry &registry,
						  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief React to a release: toggle, restyle, then call the
		 * long-press or click callback.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void release(ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Set the selection of a toggle button and restyle.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @param selected The new selection (ignored when not a toggle).
		 */
		static void setSelected(ecs::ComponentRegistry &registry,
								const ecs::Entity::Identifier &identifier,
								bool selected);

		/**
		 * @brief Write the active glyph and style to the icon child and the
		 * selection to Accessibility.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void syncContent(ecs::ComponentRegistry &registry,
								const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Run the variant system's restyle function, if any.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 */
		static void restyle(ecs::ComponentRegistry &registry,
							const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Get the glyph to show: loading, selected or regular.
		 * @param button The button state.
		 * @return The glyph name.
		 */
		static const std::string &
			activeIconGlyphName(const components::Button &button);

		/** @brief Whether disabled. @param registry It. @param identifier It.
		 * @return True when its StyleState is disabled. */
		static bool isDisabled(const ecs::ComponentRegistry &registry,
							   const ecs::Entity::Identifier &identifier);

		/** @brief Whether hovered. @param registry It. @param identifier It.
		 * @return True when a hand or the mouse hovers it. */
		static bool isHovered(const ecs::ComponentRegistry &registry,
							  const ecs::Entity::Identifier &identifier);

		/** @brief Whether pressed. @param registry It. @param identifier It.
		 * @return True when a hand or the mouse presses it. */
		static bool isPressed(const ecs::ComponentRegistry &registry,
							  const ecs::Entity::Identifier &identifier);

		/**
		 * @brief Apply the container color override, if any.
		 * @param button The button state.
		 * @param color The themed color.
		 * @return The override, or the themed color.
		 */
		static utility::graphic::Color32Bit
			resolveContainerColor(const components::Button &button,
								  const utility::graphic::Color32Bit &color);

		/**
		 * @brief Apply the content color override, if any.
		 * @param button The button state.
		 * @param color The themed color.
		 * @return The override, or the themed color.
		 */
		static utility::graphic::Color32Bit
			resolveContentColor(const components::Button &button,
								const utility::graphic::Color32Bit &color);

		/**
		 * @brief Grow the hover target to the minimum touch size.
		 * @param registry The registry holding the components.
		 * @param identifier The button entity.
		 * @param width The visual width.
		 * @param height The visual height.
		 */
		static void applyTouchTarget(ecs::ComponentRegistry &registry,
									 const ecs::Entity::Identifier &identifier,
									 float width, float height);

		/**
		 * @brief Sync the content of the button with the given identifier.
		 * @param entityIdentifier The identifier of the entity to update.
		 */
		void update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
