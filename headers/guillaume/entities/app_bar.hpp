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
#include <vector>

#include <utility/graphic/color.hpp>
#include <utility/graphic/pose.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_director.hpp"

#include "guillaume/entities/icon.hpp"
#include "guillaume/entities/builder_base.hpp"
#include "guillaume/entities/surface.hpp"
#include "guillaume/entities/text.hpp"

namespace guillaume::entities
{

	/**
	 * @brief App bar entity: a surface hosting a leading navigation icon, a
	 * title text and trailing action icons.
	 *
	 * App bars reuse `SurfaceBase` for the layout, geometry and background
	 * surface. They arrange their children along a horizontal axis, centered
	 * on the cross axis. The M3 variants are:
	 * - `Small`: height 56.
	 * - `Medium`: height 104.
	 * - `Large`: height 152.
	 * - `Bottom`: height 80.
	 *
	 * @see SurfaceBase
	 */
	class AppBar: public SurfaceBase<>
	{
		public:
		/**
		 * @brief App bar variant.
		 */
		enum class Variant { Small, Medium, Large, Bottom };

		/**
		 * @brief Builder used to configure and create `AppBar` entities.
		 */
		class Builder: public EntityBuilderBase<AppBar>
		{
			private:
			SurfaceConfig _config;					///< Surface configuration.
			Variant _variant { Variant::Small };	///< App bar variant.
			std::string _title {};					///< Title text.
			std::string _navigationIcon {};	   ///< Leading navigation icon.
			std::vector<std::string> _actions {};	 ///< Trailing action icons.

			public:
			/**
			 * @brief Construct a new AppBar Builder.
			 * @param componentRegistry The component registry.
			 * @param entityRegistry The entity registry.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Build the AppBar entity from the current configuration.
			 * @return The newly created AppBar.
			 */
			std::shared_ptr<AppBar> buildEntity(void) override;

			/**
			 * @brief Reset the builder.
			 */
			void reset(void) override;

			/**
			 * @brief Set the app bar pose.
			 * @param pose The new pose.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the app bar variant.
			 * @param variant The new variant.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withVariant(Variant variant);

			/**
			 * @brief Set the title text.
			 * @param title The new title.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTitle(const std::string &title);

			/**
			 * @brief Set the leading navigation icon glyph name.
			 * @param iconGlyphName The glyph name of the navigation icon.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withNavigationIcon(const std::string &iconGlyphName);

			/**
			 * @brief Append a trailing action icon.
			 * @param iconGlyphName The glyph name of the action icon.
			 * @return Reference to the builder for chaining.
			 */
			Builder &addAction(const std::string &iconGlyphName);

			/**
			 * @brief Force the app bar width.
			 * @param width The new width.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withFixedWidth(float width);
		};

		/**
		 * @brief Director that orchestrates `AppBar::Builder`.
		 */
		class Director: public EntityDirectorBase
		{
			public:
			/**
			 * @brief Create an app bar using the builder.
			 * @param builder The builder to configure.
			 * @param parent The parent entity.
			 * @param variant The app bar variant.
			 * @return The newly created app bar.
			 */
			std::shared_ptr<AppBar>
				makeAppBar(Builder &builder,
						   std::shared_ptr<ecs::Entity> parent,
						   Variant variant);
		};

		private:
		Variant _variant { Variant::Small };	 ///< App bar variant.
		std::string _title {};					 ///< Title text.
		std::string _navigationIcon {};			 ///< Leading navigation icon.
		std::vector<std::string> _actions {};	 ///< Trailing action icons.
		std::shared_ptr<Icon> _navigationIconEntity;	///< Leading icon child.
		std::shared_ptr<Text> _titleEntity;				///< Title child.
		std::vector<std::shared_ptr<Icon>>
			_actionEntities {};	   ///< Trailing action children.

		private:
		/**
		 * @brief Get the fixed height for the current variant.
		 * @return The app bar height.
		 */
		float getVariantHeight(void) const;

		/**
		 * @brief Apply the variant-specific geometry and colors.
		 */
		void applyVariant(void);

		/**
		 * @brief Build the leading, title and trailing child entities.
		 */
		void buildContent(void);

		/**
		 * @brief Attach the built children to the surface.
		 */
		void attachContent(void);

		public:
		/**
		 * @brief Construct an AppBar entity.
		 * @param registry The component registry.
		 * @param config The surface configuration.
		 * @param variant The app bar variant.
		 * @param title The title text.
		 * @param navigationIcon The leading navigation icon glyph name.
		 * @param actions The trailing action icon glyph names.
		 */
		AppBar(ecs::ComponentRegistry &registry, const SurfaceConfig &config,
			   Variant variant, const std::string &title,
			   const std::string &navigationIcon,
			   const std::vector<std::string> &actions);

		/**
		 * @brief Default destructor.
		 */
		~AppBar(void) override;

		/**
		 * @brief Set the app bar variant.
		 * @param variant The new variant.
		 * @return Reference to this app bar for chaining.
		 */
		AppBar &setVariant(Variant variant);

		/**
		 * @brief Get the app bar variant.
		 * @return The variant.
		 */
		Variant getVariant(void) const;

		/**
		 * @brief Set the title text.
		 * @param title The new title.
		 * @return Reference to this app bar for chaining.
		 */
		AppBar &setTitle(const std::string &title);

		/**
		 * @brief Initialize the app bar derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the app bar derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
