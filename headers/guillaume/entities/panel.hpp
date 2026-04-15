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

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/borders.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Panel component
	 */
	class Panel:
		public ecs::ParentEntityFiller<components::Transform, components::Bound,
									   components::Color, components::Borders>
	{
		public:
		/**
		 * @brief Builder used to configure and create `Panel` entities.
		 */
		class Builder: public ecs::EntityBuilder
		{
			private:
			std::unique_ptr<Panel>
				_panel;	   ///< Unique pointer to the Panel entity being built
			utility::graphic::PoseF _pose;	  ///< Pose of the panel to be used
											  ///< (position, rotation, scale)
			std::vector<ecs::Entity::Identifier>
				_entities;	  ///< Entities to be attached to the panel
			utility::graphic::Color32Bit
				_color;				///< Color of the panel to be used (RGBA)
			float _borderRadius;	///< Border radius to be used for the panel

			public:
			/**
			 * @brief Construct a new Panel Builder object.
			 * @param componentRegistry The component registry to register
			 * components to.
			 * @param entityRegistry The entity registry to register entities
			 * to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor for the Panel Builder class.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the panel entity.
			 * @return The entity identifier of the newly created panel entity.
			 */
			ecs::Entity::Identifier registerEntity(void) override;

			/**
			 * @brief Reset the builder to its initial state for creating a new
			 * Panel entity.
			 */
			void reset(void) override;

			/**
			 * @brief Set the pose of the panel to be used for the Panel entity.
			 * @param pose The pose of the panel to set (position, rotation,
			 * scale).
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the entities to be attached to the panel for the Panel
			 * entity.
			 * @param entities The entities to attach to the panel.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withEntities(
				const std::vector<ecs::Entity::Identifier> &entities);

			/**
			 * @brief Set the color of the panel to be used for the Panel
			 * entity.
			 * @param color The color of the panel to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withColor(const utility::graphic::Color32Bit &color);

			/**
			 * @brief Set the border radius of the panel to be used for the
			 * Panel entity.
			 * @param borderRadius The border radius of the panel to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withBorderRadius(float borderRadius);
		};

		/**
		 * @brief Director that orchestrates `Panel::Builder` to create
		 * preconfigured panel entities.
		 */
		class Director: public ecs::EntityDirector
		{
			public:
			/**
			 * @brief Construct a new Panel Director object.
			 */
			Director(void);

			/**
			 * @brief Default destructor for the Panel Director class.
			 */
			~Director(void);

			/**
			 * @brief Create a default panel entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the default panel.
			 * @param pose The pose to set for the default panel.
			 * @param entities The entities to attach to the default
			 * panel.
			 * @return The entity identifier of the newly created panel entity.
			 */
			ecs::Entity::Identifier makeDefaultPanel(
				Builder &builder, const utility::graphic::PoseF &pose,
				const std::vector<ecs::Entity::Identifier> &entities);

			/**
			 * @brief Create a color panel entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the color panel.
			 * @param pose The pose to set for the color panel.
			 * @param color The color to set for the color panel.
			 * @param entities The entities to attach to the color panel.
			 * @return The entity identifier of the newly created color panel
			 * entity.
			 */
			ecs::Entity::Identifier makeColorPanel(
				Builder &builder, const utility::graphic::PoseF &pose,
				const utility::graphic::Color32Bit &color,
				const std::vector<ecs::Entity::Identifier> &entities);
		};

		private:
		utility::graphic::PoseF
			_pose {};	 ///< Pose to be used for creating panel entities
						 ///< (position, rotation, scale)
		utility::graphic::Color32Bit
			_color {};	  ///< Color to be used for creating panel entities
						  ///< (RGBA)
		float _borderRadius { 16.0f };	  ///< Border radius to be used for
										  ///< creating panel entities
		std::vector<ecs::Entity::Identifier>
			_entities {};	 ///< Entities to be attached to created panel
							 ///< entities

		public:
		/**
		 * @brief Default constructor for the Panel component.
		 * @param registry Reference to the component registry for
		 * initializing components.
		 * @param pose The pose to initialize the Panel component with
		 * (position, rotation, scale).
		 * @param color The color to initialize the Panel component with (RGBA).
		 * @param borderRadius The border radius to initialize the Panel
		 * @param entities The entities to attach to the panel for this
		 * Panel component with.
		 */
		Panel(ecs::ComponentRegistry &registry,
			  const utility::graphic::PoseF &pose,
			  const utility::graphic::Color32Bit &color, float borderRadius,
			  const std::vector<ecs::Entity::Identifier> &entities);

		/**
		 * @brief Default destructor for the Panel component.
		 */
		~Panel(void);

		/**
		 * @brief Set the pose of the panel for this Panel entity.
		 * @param pose The new pose to set for the panel (position, rotation,
		 * scale).
		 * @return Reference to this Panel for chaining.
		 */
		Panel &setPose(const utility::graphic::PoseF &pose);

		/**
		 * @brief Set the color of the panel for this Panel entity.
		 * @param color The new color to set for the panel (RGBA).
		 * @return Reference to this Panel for chaining.
		 */
		Panel &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Set the border radius of the panel for this Panel entity.
		 * @param borderRadius The new border radius to set for the panel.
		 * @return Reference to this Panel for chaining.
		 */
		Panel &setBorderRadius(float borderRadius);

		/**
		 * @brief Set the entities to be attached to the panel for this
		 * Panel entity.
		 * @param entities The new entities to attach to the panel.
		 * @return Reference to this Panel for chaining.
		 */
		Panel &
			setEntities(const std::vector<ecs::Entity::Identifier> &entities);

		/**
		 * @brief Recompute the panel entity's derived state.
		 */
		void update(void) override;
	};
};	  // namespace guillaume::entities
