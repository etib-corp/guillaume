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
	 * @brief Tooltip entity: a small overlay-attached label.
	 *
	 * Tooltips reuse `SurfaceBase` for the layout, geometry and background
	 * surface and `components::Overlay` for the transient overlay behaviour.
	 * The M3 variants are:
	 * - `Plain`: a short single-line text on an inverse surface, pill radius 4.
	 * - `Rich`: a larger title (font 16) followed by a supporting text
	 *   (font 14), corner radius 12.
	 *
	 * The class is a facade without fields: its state is
	 * components::Tooltip and systems::Tooltip styles the tooltip, runs the
	 * hover delays and places it. `build()` only builds the child
	 * entities.
	 *
	 * @see SurfaceBase
	 * @see components::Overlay
	 */
	class Tooltip:
		public SurfaceBase<components::Overlay, components::Elevation,
						   components::Animation, components::Accessibility,
						   components::Tooltip>
	{
		public:
		/// Tooltip variant.
		using Variant = components::Tooltip::Variant;

		/**
		 * @brief Paper `enterTouchDelay`, in seconds.
		 */
		static constexpr float DefaultEnterDelay =
			components::Tooltip::DefaultEnterDelay;

		/**
		 * @brief Paper `leaveTouchDelay`, in seconds.
		 */
		static constexpr float DefaultLeaveDelay =
			components::Tooltip::DefaultLeaveDelay;

		/**
		 * @brief Builder used to configure and create `Tooltip` entities.
		 */
		class Builder: public EntityBuilderBase<Tooltip>
		{
			private:
			SurfaceConfig _config;					///< Surface configuration.
			Variant _variant { Variant::Plain };	///< Tooltip variant.
			std::string _text {};					///< Tooltip body text.
			std::string _title {};	  ///< Tooltip title (Rich only).

			public:
			/**
			 * @brief Construct a new Tooltip Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the Tooltip entity from the current configuration.
			 * @return The newly created Tooltip.
			 */
			std::shared_ptr<Tooltip> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the tooltip pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the tooltip variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the tooltip body text.
			 * @param text The new body text.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withText(const std::string &text);

			/**
			 * @brief Set the tooltip title (Rich variant only).
			 * @param title The new title.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTitle(const std::string &title);

			/**
			 * @brief Force the tooltip width.
			 * @param width The new fixed width.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withFixedWidth(float width);

			/**
			 * @brief Create a tooltip using the builder.
			 * @param parent The parent entity.
			 * @param variant The tooltip variant.
			 * @param text The tooltip body text.
			 * @param title The tooltip title (Rich variant only).
			 * @return The newly created tooltip.
			 */
			std::shared_ptr<Tooltip>
				makeTooltip(std::shared_ptr<ecs::Entity> parent,
							Variant variant, const std::string &text,
							const std::string &title = "");
		};

		public:
		/**
		 * @brief Create the entity, fully built.
		 *
		 * Constructors are private: an entity must be owned before it can
		 * parent its children, so construction and child creation happen
		 * together here.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments.
		 * @return The new entity.
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
		 * @brief Set the tooltip variant.
		 * @param variant The new variant.
		 * @return Reference to this tooltip for chaining.
		 */
		Tooltip &setVariant(Variant variant);

		/**
		 * @brief Get the tooltip variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the tooltip body text.
		 * @param text The new body text.
		 * @return Reference to this tooltip for chaining.
		 */
		Tooltip &setText(const std::string &text);

		/**
		 * @brief Attach the tooltip to the entity whose hover triggers it.
		 *
		 * The anchor must own a mouse or hand hover interaction. Hovering it
		 * for the enter delay shows the tooltip under the anchor; leaving it
		 * for the leave delay hides it.
		 *
		 * @param anchor Trigger entity (nullptr detaches).
		 * @return Reference to this tooltip for chaining.
		 */
		Tooltip &setAnchor(std::shared_ptr<ecs::Entity> anchor);

		/**
		 * @brief Set the hover-in delay (Paper `enterTouchDelay`).
		 * @param seconds Delay, must be non-negative.
		 * @return Reference to this tooltip for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Tooltip &setEnterDelay(float seconds);

		/**
		 * @brief Set the hover-out delay (Paper `leaveTouchDelay`).
		 * @param seconds Delay, must be non-negative.
		 * @return Reference to this tooltip for chaining.
		 * @throws std::invalid_argument If @p seconds is negative.
		 */
		Tooltip &setLeaveDelay(float seconds);

		/** @brief Get the hover-in delay. @return Seconds. */
		float getEnterDelay(void) const;

		/** @brief Get the hover-out delay. @return Seconds. */
		float getLeaveDelay(void) const;

		/**
		 * @brief Set the area the tooltip is kept inside.
		 * @param width Area width.
		 * @param height Area height.
		 * @return Reference to this tooltip for chaining.
		 */
		Tooltip &setViewport(float width, float height);

		/** @brief Show the tooltip under its anchor. @return *this. */
		Tooltip &show(void);

		/** @brief Hide the tooltip. @return *this. */
		Tooltip &hide(void);

		/** @brief Check the visibility. @return True when shown. */
		bool isVisible(void) const;

		/**
		 * @brief Advance the hover delays.
		 *
		 * Driven every frame by the tooltip's own looping animation; exposed
		 * for tests and custom schedulers.
		 *
		 * @param deltaTime Elapsed seconds.
		 */
		void advance(float deltaTime);

		/**
		 * @brief Place the tooltip under (or, when it would overflow the
		 * viewport, above) its anchor, horizontally centered and clamped.
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
		 * @brief Construct a Tooltip entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The tooltip variant.
		 * @param text The tooltip body text.
		 * @param title The tooltip title (Rich variant only).
		 */
		Tooltip(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
				Variant variant, const std::string &text,
				const std::string &title);

		/**
		 * @brief Create the child entities.
		 *
		 * Called once by create(), when the entity is owned and can be given
		 * as parent to its children.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
