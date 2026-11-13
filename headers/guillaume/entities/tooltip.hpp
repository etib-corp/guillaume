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
#include <memory>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"

#include "guillaume/components/accessibility.hpp"
#include "guillaume/components/animation.hpp"
#include "guillaume/components/elevation.hpp"
#include "guillaume/components/overlay.hpp"
#include "guillaume/components/tooltip.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A small label that appears next to another entity while the
	 * pointer or a hand hovers that entity.
	 *
	 * Its state lives in components::Tooltip (variant, text, title, anchor,
	 * hover delays and their timers, visibility). systems::Tooltip styles
	 * the tooltip and arranges its children each frame, in the Layout
	 * phase; it also holds the rules that run the hover delays and place
	 * the tooltip. The class itself holds no field: its setters write the
	 * component.
	 *
	 * @code
	 * Tooltip::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto tooltip = builder.makeTooltip(parent, Tooltip::Variant::Plain,
	 * 	"Save the document");
	 * tooltip->setAnchor(saveButton);
	 * @endcode
	 *
	 * `Tooltip::create(registry, config, variant, text, title)` builds the
	 * same widget without registering it in an entity registry.
	 *
	 * Showing and hiding: a tooltip is created hidden and does nothing
	 * until setAnchor() gives it an anchor. From then on it shows itself
	 * once the anchor has been hovered for the enter delay (0.5 second by
	 * default) and hides itself once the anchor has been left for the
	 * leave delay (1.5 seconds by default). show() and hide() switch it by
	 * hand, and systems::Overlay hides it when the user presses Escape
	 * while it is the topmost visible overlay. There is no dismiss
	 * callback.
	 *
	 * The delays are counted by the tooltip's own components::Animation, a
	 * looping one-second animation that systems::Animation samples every
	 * frame, in the Event phase, and that setAnchor() starts.
	 *
	 * The variants are:
	 * - `Plain`: one text, in a 32 dp high label on the inverse surface
	 *   color.
	 * - `Rich`: a title (TitleSmall type role) above the text, in a 64 dp
	 *   high, elevated label on a surface container color.
	 *
	 * Things to know:
	 * - The anchor needs a components::MouseHoverInteraction or a
	 *   components::HandHoverInteraction, and must be enabled, for its
	 *   hover to be seen.
	 * - The tooltip stores the identifier of its anchor, not the entity:
	 *   it does not keep the anchor alive.
	 * - The tooltip is placed when it is shown, not every frame: it does
	 *   not follow an anchor that moves while it is visible.
	 * - It is only kept inside the screen when setViewport() gave it an
	 *   area.
	 * - A hidden tooltip is painted transparent.
	 * - systems::Tooltip overwrites the layout settings, height, color,
	 *   corner radius and elevation every frame.
	 * - The title is fixed at creation.
	 *
	 * @see components::Tooltip, systems::Tooltip, systems::Overlay
	 */
	class Tooltip:
		public SurfaceBase<components::Overlay, components::Elevation,
						   components::Animation, components::Accessibility,
						   components::Tooltip>
	{
		public:
		/// Look of the tooltip: Plain (one text) or Rich (title and text).
		using Variant = components::Tooltip::Variant;

		/**
		 * @brief Default time the anchor must be hovered before the tooltip
		 * shows: 0.5 second (Paper `enterTouchDelay`).
		 */
		static constexpr float DefaultEnterDelay =
			components::Tooltip::DefaultEnterDelay;

		/**
		 * @brief Default time after the anchor is left before the tooltip
		 * hides: 1.5 seconds (Paper `leaveTouchDelay`).
		 */
		static constexpr float DefaultLeaveDelay =
			components::Tooltip::DefaultLeaveDelay;

		/**
		 * @brief Configures a Tooltip, then builds it and registers it in
		 * the entity registry given at construction.
		 */
		class Builder: public EntityBuilderBase<Tooltip>
		{
			private:
			SurfaceConfig _config;	  ///< Pose, layout and optional width.
			Variant _variant { Variant::Plain };	///< Tooltip variant.
			std::string _text {};					///< Body text.
			std::string _title {};	  ///< Title, used by the Rich variant.

			public:
			/**
			 * @brief Create a builder set to the defaults of reset().
			 * @param componentRegistry The registry the tooltip's components
			 * are created in.
			 * @param entityRegistry The registry the built tooltips are
			 * added to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a tooltip and its children from the current
			 * configuration, without registering it or setting its parent.
			 * @return The new tooltip, hidden and without anchor.
			 */
			std::shared_ptr<Tooltip> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: Plain variant, no text, no title,
			 * a default pose and a width that follows the content.
			 */
			void reset(void) override;

			/**
			 * @brief Set the pose the tooltip is created with.
			 * @param pose The position and orientation of the label.
			 * Default: a default-constructed pose. Showing an anchored
			 * tooltip replaces it.
			 * @return This builder, for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the tooltip variant.
			 * @param variant The variant. Default: `Variant::Plain`.
			 * @return This builder, for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the body text.
			 * @param text The text. Default: empty, which builds no text
			 * child.
			 * @return This builder, for chaining.
			 */
			Builder &withText(const std::string &text);

			/**
			 * @brief Set the title shown above the text.
			 * @param title The title. Used by the Rich variant only; an
			 * empty title builds no title child. Default: empty.
			 * @return This builder, for chaining.
			 */
			Builder &withTitle(const std::string &title);

			/**
			 * @brief Give the tooltip a fixed width.
			 * @param width The width, in dp. Default: none, the width
			 * follows the content.
			 * @return This builder, for chaining.
			 */
			Builder &withFixedWidth(float width);

			/**
			 * @brief Build a tooltip, register it under @p parent and reset
			 * the builder.
			 *
			 * The arguments replace the variant, text and title set before;
			 * the pose and fixed width are kept.
			 * @param parent The entity the tooltip is attached to. It is
			 * not the anchor: call Tooltip::setAnchor() for that.
			 * @param variant The tooltip variant.
			 * @param text The body text.
			 * @param title The title, used by the Rich variant only.
			 * @return The registered tooltip, hidden and without anchor.
			 */
			std::shared_ptr<Tooltip>
				makeTooltip(std::shared_ptr<ecs::Entity> parent,
							Variant variant, const std::string &text,
							const std::string &title = "");
		};

		public:
		/**
		 * @brief Create a tooltip and its children, without registering it
		 * in an entity registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * (registry, config, variant, text, title). The children are built
		 * here, by build(), because a child needs its parent to be owned by
		 * a shared pointer.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new tooltip, hidden, with no parent and no anchor.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Tooltip> create(Arguments &&...arguments)
		{
			std::shared_ptr<Tooltip> entity(
				new Tooltip(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Default destructor.
		 */
		~Tooltip(void) override = default;

		/**
		 * @brief Change the tooltip variant.
		 *
		 * Builds the title child when the new variant is Rich, the title is
		 * not empty and the child does not exist yet, then restyles at
		 * once. Switching back to Plain takes the title out of the layout.
		 * @param variant The new variant.
		 * @return This tooltip, for chaining.
		 */
		Tooltip &setVariant(Variant variant);

		/**
		 * @brief Get the tooltip variant.
		 * @return The variant stored in components::Tooltip.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Change the body text.
		 *
		 * Builds the text child when the tooltip had none and @p text is
		 * not empty, then restyles at once.
		 * @param text The new body text.
		 * @return This tooltip, for chaining.
		 */
		Tooltip &setText(const std::string &text);

		/**
		 * @brief Choose the entity whose hover shows the tooltip.
		 *
		 * Resets the hover timers and starts the animation that counts the
		 * delays. From then on, hovering the anchor for the enter delay
		 * shows the tooltip next to it, and leaving it for the leave delay
		 * hides it. The anchor needs a components::MouseHoverInteraction or
		 * a components::HandHoverInteraction. Only its identifier is
		 * stored.
		 * @param anchor The entity to watch. `nullptr` detaches the
		 * tooltip and stops the animation; a tooltip that is shown stays
		 * shown until hide() is called.
		 * @return This tooltip, for chaining.
		 */
		Tooltip &setAnchor(std::shared_ptr<ecs::Entity> anchor);

		/**
		 * @brief Set how long the anchor must be hovered before the tooltip
		 * shows (Paper `enterTouchDelay`).
		 * @param seconds The delay, in seconds, 0 or more. Default:
		 * DefaultEnterDelay.
		 * @return This tooltip, for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Tooltip &setEnterDelay(float seconds);

		/**
		 * @brief Set how long the tooltip stays shown after the anchor is
		 * left (Paper `leaveTouchDelay`).
		 * @param seconds The delay, in seconds, 0 or more. Default:
		 * DefaultLeaveDelay.
		 * @return This tooltip, for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Tooltip &setLeaveDelay(float seconds);

		/**
		 * @brief Get how long the anchor must be hovered before the tooltip
		 * shows.
		 * @return The delay, in seconds.
		 */
		float getEnterDelay(void) const;

		/**
		 * @brief Get how long the tooltip stays shown after the anchor is
		 * left.
		 * @return The delay, in seconds.
		 */
		float getLeaveDelay(void) const;

		/**
		 * @brief Set the area the tooltip must stay inside, usually the
		 * window.
		 *
		 * The area starts at (0, 0), in the coordinates of the anchor's
		 * pose. Places the tooltip again when it is shown. Without an area,
		 * the tooltip is always placed under its anchor, unclamped.
		 * @param width The width of the area, in dp.
		 * @param height The height of the area, in dp.
		 * @return This tooltip, for chaining.
		 */
		Tooltip &setViewport(float width, float height);

		/**
		 * @brief Show the tooltip at once, without waiting for the enter
		 * delay.
		 *
		 * Places it next to its anchor (see place()) and restyles at once.
		 * Without an anchor it is shown where it is. While the anchor is
		 * not hovered, the leave delay hides it again.
		 * @return This tooltip, for chaining.
		 */
		Tooltip &show(void);

		/**
		 * @brief Hide the tooltip at once, without waiting for the leave
		 * delay. Restyles at once.
		 * @return This tooltip, for chaining.
		 */
		Tooltip &hide(void);

		/**
		 * @brief Check whether the tooltip is shown.
		 * @return True when shown. False for a tooltip that was never
		 * shown.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Advance the hover delays by hand.
		 *
		 * The tooltip's own looping animation calls this every frame once
		 * an anchor is set; call it yourself only in tests or when you
		 * drive time some other way. Shows the tooltip once the anchor has
		 * been hovered for the enter delay and hides it once the anchor has
		 * been left for the leave delay. Does nothing without a live
		 * anchor.
		 * @param deltaTime The time elapsed since the last call, in
		 * seconds.
		 */
		void advance(float deltaTime);

		/**
		 * @brief Move the tooltip next to its anchor.
		 *
		 * The tooltip is centered horizontally on the anchor and placed
		 * just under it. When setViewport() gave an area, it goes above
		 * the anchor if it would overflow the bottom of the area, and is
		 * clamped to the area. Called by show(); call it again after
		 * moving the anchor. Does nothing when the anchor is missing or has
		 * no Transform and Bound.
		 */
		void place(void);

		private:
		/**
		 * @brief Build the title and body child entities that are missing
		 * and record their identifiers in components::Tooltip.
		 *
		 * Does nothing until the tooltip is owned by a shared pointer.
		 */
		void buildContent(void);

		private:
		/**
		 * @brief Construct a tooltip, without its children.
		 * @param registry The registry the tooltip's components are created
		 * in.
		 * @param config Pose and layout of the label. systems::Tooltip
		 * overwrites the layout settings and the height.
		 * @param variant The tooltip variant.
		 * @param text The body text.
		 * @param title The title, used by the Rich variant only.
		 */
		Tooltip(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				Variant variant, const std::string &text,
				const std::string &title);

		/**
		 * @brief Build the children and restyle.
		 *
		 * Called once by create(), when the tooltip is owned by a shared
		 * pointer and can be given as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
