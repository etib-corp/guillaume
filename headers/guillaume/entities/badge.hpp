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
#include <optional>
#include <string>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/components/animation.hpp"

#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Material 3 badge (react-native-paper `Badge`).
	 *
	 * Defaults to Paper's 20 dp label badge; `Variant::Dot` keeps the 6 dp
	 * status dot. Visibility changes fade the badge over 150 ms (M3 short3).
	 */
	class Badge: public SurfaceBase<components::Animation>
	{
		public:
		/**
		 * @brief Badge variants.
		 */
		enum class Variant {
			Dot,	  ///< 6 dp dot without label.
			Small,	  ///< 16 dp label badge.
			Large	  ///< 20 dp label badge (Paper default).
		};

		/**
		 * @brief Builder for Badge entities.
		 */
		class Builder: public EntityBuilderBase<Badge>
		{
			private:
			SurfaceConfig _config;					///< Surface configuration.
			Variant _variant { Variant::Large };	///< Badge variant.
			std::string _label {};					///< Badge label.

			public:
			/**
			 * @brief Construct a badge builder.
			 * @param componentRegistry Component registry.
			 * @param entityRegistry Entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the badge.
			 * @return The badge.
			 */
			std::shared_ptr<Badge> buildEntity(void) override;

			/**
			 * @brief Reset to the Paper defaults.
			 */
			void reset(void) override;

			/** @brief Set the pose. @param pose Pose. @return *this. */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/** @brief Set the variant. @param variant Variant. @return *this.
			 */
			Builder &withVariant(Variant variant);

			/** @brief Set the label. @param label Label. @return *this. */
			Builder &withLabel(const std::string &label);
		};

		/**
		 * @brief Director for Badge entities.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Build a badge.
			 * @param builder Builder.
			 * @param parent Parent entity.
			 * @param label Label.
			 * @param variant Variant.
			 * @return The badge.
			 */
			std::shared_ptr<Badge>
				makeBadge(Builder &builder, std::shared_ptr<ecs::Entity> parent,
						  const std::string &label, Variant variant);
		};

		/**
		 * @brief Construct a badge.
		 * @param registry Component registry.
		 * @param config Surface configuration.
		 * @param variant Variant.
		 * @param label Label.
		 */
		Badge(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			  Variant variant, const std::string &label);

		/**
		 * @brief Destructor.
		 */
		~Badge(void) override;

		/** @brief Set the variant. @param variant Variant. @return *this. */
		Badge &setVariant(Variant variant);

		/** @brief Get the variant. @return The variant. */
		Variant getVariant(void) const;

		/** @brief Set the label. @param label Label. @return *this. */
		Badge &setLabel(const std::string &label);

		/**
		 * @brief Override the variant size (Paper `size`).
		 * @param size Badge height in dp, must be positive.
		 * @return *this.
		 * @throws std::invalid_argument If @p size is not positive.
		 */
		Badge &setSize(float size);

		/**
		 * @brief Drop the size override.
		 * @return *this.
		 */
		Badge &clearSize(void);

		/**
		 * @brief Get the resolved badge height.
		 * @return The size, in dp.
		 */
		float getSize(void) const;

		/**
		 * @brief Override the container color (default `Error`).
		 * @param color Container color.
		 * @return *this.
		 */
		Badge &setContainerColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Override the label color (default `OnError`).
		 * @param color Label color.
		 * @return *this.
		 */
		Badge &setContentColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Show or hide the badge with a 150 ms fade (Paper `visible`).
		 * @param visible Target visibility.
		 * @return *this.
		 */
		Badge &setVisible(bool visible);

		/** @brief Get the target visibility. @return True when visible. */
		bool isVisible(void) const;

		/**
		 * @brief Get the current fade opacity.
		 * @return Opacity in [0, 1].
		 */
		float getOpacity(void) const;

		/**
		 * @brief Build the label child.
		 */
		void initialize(void) override;

		/**
		 * @brief Restyle the badge.
		 */
		void update(void) override;

		private:
		Variant _variant { Variant::Large };	///< Badge variant.
		std::string _label {};					///< Badge label.
		std::shared_ptr<Text> _labelEntity;		///< Label child entity.
		std::optional<float> _size {};			///< Size override.
		std::optional<utility::graphic::Color32Bit>
			_containerColor {};	   ///< Container color override.
		std::optional<utility::graphic::Color32Bit>
			_contentColor {};		 ///< Label color override.
		bool _visible { true };		 ///< Target visibility.
		float _opacity { 1.0f };	 ///< Current fade opacity.
		float _fadeFrom { 1.0f };	 ///< Opacity at the start of the fade.

		/**
		 * @brief Apply geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Get the size of the current variant.
		 * @return The size, in dp.
		 */
		float getVariantSize(void) const;
	};

}	 // namespace guillaume::entities
