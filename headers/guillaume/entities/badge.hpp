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

#include "guillaume/components/animation.hpp"
#include "guillaume/components/badge.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief A small colored pill showing a count or a short label, or a
	 * plain status dot (react-native-paper `Badge`).
	 *
	 * Its state lives in components::Badge (variant, label, size and color
	 * overrides, visibility and fade opacity); systems::Badge sizes and
	 * colors the pill and styles its label each frame, in the Layout
	 * phase. components::Animation drives the 150 ms fade played when the
	 * visibility changes. The class itself holds no field: its setters
	 * write the component and restyle at once. The label is a Text child
	 * owned by the badge.
	 *
	 * @code
	 * Badge::Builder builder(getComponentRegistry(), getEntityRegistry());
	 * auto badge = builder.makeBadge(parent, "3", Badge::Variant::Large);
	 * badge->setLabel("4");
	 * badge->setVisible(false);
	 * @endcode
	 *
	 * `Badge::create(registry, config, variant, label)` builds the same
	 * widget, label included, without registering it in an entity registry.
	 *
	 * Watch out:
	 * - The label child is created only at construction, and only when the
	 *   variant is not `Dot` and the label is not empty. A badge built
	 *   without it never shows text: setLabel() and setVariant() update an
	 *   existing label child but do not create one.
	 * - The height is the size of the variant (6, 16 or 20 dp) or the
	 *   override of setSize(). The width is the height, or more when the
	 *   label is wider: the label width plus 3 dp on each side.
	 * - systems::Badge overwrites the fill color, the corner radius, the
	 *   padding, the spacing and the fixed size every frame. Use
	 *   setContainerColor() and setSize(); SurfaceBase::setColor() and
	 *   SurfaceBase::setBorderRadius() do not last.
	 * - The default colors are the Error and OnError roles of the active
	 *   theme.
	 *
	 * @see components::Badge, systems::Badge, SurfaceBase
	 */
	class Badge: public SurfaceBase<components::Animation, components::Badge>
	{
		public:
		/// Size class of the badge: `Dot` (6 dp, no label), `Small` (16 dp)
		/// or `Large` (20 dp).
		using Variant = components::Badge::Variant;

		/**
		 * @brief Configures Badge widgets and registers them in the scene's
		 * entity registry.
		 */
		class Builder: public EntityBuilderBase<Badge>
		{
			private:
			SurfaceConfig _config;	  ///< Pose and layout of the badge surface.
			Variant _variant { Variant::Large };	///< Size class.
			std::string _label {};	  ///< Text shown in the badge.

			public:
			/**
			 * @brief Construct a builder holding the defaults of reset().
			 * @param componentRegistry The registry the components of the
			 * built badges are stored in.
			 * @param entityRegistry The registry that owns the badges built
			 * by `makeBadge`.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build a badge and its label from the current
			 * configuration, without registering it or giving it a parent.
			 * @return The new badge.
			 */
			std::shared_ptr<Badge> buildEntity(void) override;

			/**
			 * @brief Restore the defaults: default pose, the `Large`
			 * variant, an empty label, and a horizontal layout centered on
			 * both axes.
			 */
			void reset(void) override;

			/**
			 * @brief Set where the badge is.
			 * @param pose The pose of the top-left corner of the badge. A
			 * default-constructed pose by default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Choose the size class of the badge.
			 * @param variant `Dot`, `Small` or `Large`. `Large` by
			 * default.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the text shown in the badge.
			 * @param label The label, such as a count. Empty by default,
			 * which builds a badge without a label child.
			 * @return Reference to this builder for chaining.
			 */
			Builder &withLabel(const std::string &label);

			/**
			 * @brief Build a badge and its label, register it under
			 * `parent` and reset the builder.
			 * @param parent The entity the badge is attached to, or nullptr
			 * for a root entity.
			 * @param label The text shown in the badge. Ignored by the
			 * `Dot` variant.
			 * @param variant The size class: `Dot`, `Small` or `Large`.
			 * @return The registered badge.
			 */
			std::shared_ptr<Badge>
				makeBadge(std::shared_ptr<ecs::Entity> parent,
						  const std::string &label, Variant variant);
		};

		/**
		 * @brief Build a badge without registering it in an entity
		 * registry.
		 *
		 * The arguments are forwarded to the private constructor:
		 * `(registry, config, variant, label)`. It then calls build(),
		 * which creates the label child when the badge shows one.
		 * @tparam Arguments The constructor argument types.
		 * @param arguments The constructor arguments, as listed above.
		 * @return The new badge. The caller owns it; the badge owns its
		 * label.
		 */
		template<typename... Arguments>
		static std::shared_ptr<Badge> create(Arguments &&...arguments)
		{
			std::shared_ptr<Badge> entity(
				new Badge(std::forward<Arguments>(arguments)...));
			entity->build();
			return entity;
		}

		/**
		 * @brief Destructor.
		 */
		~Badge(void) override;

		/**
		 * @brief Change the size class of the badge. Resizes at once.
		 *
		 * It does not create or remove the label child.
		 * @param variant `Dot` (6 dp), `Small` (16 dp) or `Large` (20 dp).
		 * @return Reference to this badge for chaining.
		 */
		Badge &setVariant(Variant variant);

		/**
		 * @brief Get the size class of the badge.
		 * @return The variant stored in components::Badge.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Change the text shown in the badge. Restyles at once.
		 *
		 * The text only shows when the badge was built with a label child
		 * (see the class description).
		 * @param label The new label.
		 * @return Reference to this badge for chaining.
		 */
		Badge &setLabel(const std::string &label);

		/**
		 * @brief Override the height of the variant (Paper `size`).
		 * Resizes at once.
		 *
		 * The label font size follows: it is half the height.
		 * @param size The badge height, in dp. Must be positive.
		 * @return Reference to this badge for chaining.
		 * @throws std::invalid_argument If @p size is zero or negative.
		 */
		Badge &setSize(float size);

		/**
		 * @brief Drop the height override and use the height of the
		 * variant again. Resizes at once.
		 * @return Reference to this badge for chaining.
		 */
		Badge &clearSize(void);

		/**
		 * @brief Get the height the badge is drawn with.
		 * @return The override of setSize() when there is one, the height
		 * of the variant otherwise, in dp.
		 */
		float getSize(void) const;

		/**
		 * @brief Override the fill color of the pill. Restyles at once.
		 * @param color The new fill color (RGBA). Without an override the
		 * Error color of the active theme is used.
		 * @return Reference to this badge for chaining.
		 */
		Badge &setContainerColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Override the color of the label. Restyles at once.
		 * @param color The new label color (RGBA). Without an override the
		 * OnError color of the active theme is used.
		 * @return Reference to this badge for chaining.
		 */
		Badge &setContentColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Show or hide the badge with a 150 ms fade (Paper
		 * `visible`).
		 *
		 * The fade starts from the current opacity, so it can be reversed
		 * midway. Passing the current visibility does nothing. A hidden
		 * badge is fully transparent but keeps its place in a layout.
		 * @param visible True to fade the badge in, false to fade it out.
		 * @return Reference to this badge for chaining.
		 */
		Badge &setVisible(bool visible);

		/**
		 * @brief Get the visibility the badge is fading to.
		 * @return True when the badge is shown or fading in, false when it
		 * is hidden or fading out. A new badge is visible.
		 */
		bool isVisible(void) const;

		/**
		 * @brief Get the current opacity of the fade.
		 * @return The opacity, from 0 (hidden) to 1 (fully shown). It
		 * multiplies the alpha of the fill and label colors.
		 */
		float getOpacity(void) const;

		private:
		/**
		 * @brief Construct a badge, set up its fade animation and style it.
		 * @param registry The registry the components are stored in.
		 * @param config The pose and layout of the badge surface. Its
		 * color, radius, padding, spacing and fixed size are replaced by
		 * systems::Badge.
		 * @param variant The size class: `Dot`, `Small` or `Large`.
		 * @param label The text shown in the badge.
		 */
		Badge(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			  Variant variant, const std::string &label);

		/**
		 * @brief Create the label child when the badge shows one, then
		 * style the badge.
		 *
		 * Called once by create(), when the badge is held by a shared
		 * pointer and can be given as parent to its label. The label is
		 * registered in the badge itself, which owns it.
		 */
		void build(void);
	};

}	 // namespace guillaume::entities
