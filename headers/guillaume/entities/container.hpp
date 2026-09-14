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

#include <cstdint>
#include <memory>
#include <vector>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/parent_entity_filler.hpp"

#include "guillaume/components/transform.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/color.hpp"
#include "guillaume/components/borders.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Container component
	 *
	 * A `Container` is a non-interactive container surface (background color
	 * and border radius) that owns a set of child entities and arranges them in
	 * a row or a column. Children are spaced by a configurable gap, the
	 * container adds a padding around its children and a margin around its own
	 * bound, and it lifts its children toward the camera by one layer depth
	 * step so that they never z-fight with the container surface.
	 */
	class Container:
		public std::enable_shared_from_this<Container>,
		public ecs::ParentEntityFiller<components::Transform, components::Bound,
									   components::Color, components::Borders>
	{
		public:
		/**
		 * @brief Direction in which the container arranges its children.
		 */
		enum class Direction {
			Row,	  ///< Arrange children left to right.
			Column	  ///< Arrange children top to bottom.
		};

		/**
		 * @brief Builder used to configure and create `Container` entities.
		 */
		class Builder: public ecs::EntityBuilder
		{
			private:
			std::shared_ptr<Container>
				_container;	   ///< Unique pointer to the Container entity being
							   ///< built
			utility::graphic::PoseF _pose;	  ///< Pose of the container to be
											  ///< used (position, rotation,
											  ///< scale)
			utility::graphic::Color32Bit
				_color;	   ///< Color of the container to be used (RGBA)
			float _borderRadius;	///< Border radius to be used for the
									///< container
			float _padding;			///< Padding to be used around the children
			float _spacing;			///< Spacing to be used between children
			float _margin;			///< Margin to be used around the container
			Direction
				_direction;	   ///< Direction in which children are arranged
			std::vector<std::shared_ptr<ecs::Entity>>
				_entities;	  ///< Entities to be attached to the container

			public:
			/**
			 * @brief Construct a new Container Builder object.
			 * @param componentRegistry The component registry to register
			 * components to.
			 * @param entityRegistry The entity registry to register entities
			 * to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor for the Container Builder class.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the container entity.
			 * @param parent The parent entity to which the new container entity
			 * will be attached.
			 * @return A shared pointer to the newly created container entity.
			 */
			std::shared_ptr<Container>
				registerEntity(std::shared_ptr<Entity> parent);

			/**
			 * @brief Reset the builder to its initial state for creating a new
			 * Container entity.
			 */
			void reset(void) override;

			/**
			 * @brief Set the pose of the container to be used for the Container
			 * entity.
			 * @param pose The pose of the container to set (position, rotation,
			 * scale).
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPose(const utility::graphic::PoseF &pose);

			/**
			 * @brief Set the color of the container to be used for the
			 * Container entity.
			 * @param color The color of the container to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withColor(const utility::graphic::Color32Bit &color);

			/**
			 * @brief Set the border radius of the container to be used for the
			 * Container entity.
			 * @param borderRadius The border radius of the container to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withBorderRadius(float borderRadius);

			/**
			 * @brief Set the padding of the container to be used for the
			 * Container entity.
			 * @param padding The padding of the container to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withPadding(float padding);

			/**
			 * @brief Set the spacing between children for the Container entity.
			 * @param spacing The spacing between children to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withSpacing(float spacing);

			/**
			 * @brief Set the margin around the container for the Container
			 * entity.
			 * @param margin The margin around the container to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withMargin(float margin);

			/**
			 * @brief Set the direction in which children are arranged for the
			 * Container entity.
			 * @param direction The direction to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withDirection(Direction direction);

			/**
			 * @brief Set the entities to be attached to the container for the
			 * Container entity.
			 * @param entities The entities to attach to the container.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withEntities(
				const std::vector<std::shared_ptr<ecs::Entity>> &entities);
		};

		/**
		 * @brief Director that orchestrates `Container::Builder` to create
		 * preconfigured container entities.
		 */
		class Director: public ecs::EntityDirector
		{
			public:
			/**
			 * @brief Construct a new Container Director object.
			 */
			Director(void);

			/**
			 * @brief Default destructor for the Container Director class.
			 */
			~Director(void);

			/**
			 * @brief Create a default container entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the default container.
			 * @param parent The parent entity to which the new container entity
			 * will be attached.
			 * @param pose The pose to set for the default container.
			 * @param entities The entities to attach to the default
			 * container.
			 * @return A shared pointer to the newly created default container
			 * entity.
			 */
			std::shared_ptr<Container> makeDefaultContainer(
				Builder &builder, std::shared_ptr<Entity> parent,
				const utility::graphic::PoseF &pose,
				const std::vector<std::shared_ptr<ecs::Entity>> &entities);

			/**
			 * @brief Create a color container entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the color container.
			 * @param parent The parent entity to which the new color container
			 * entity will be attached.
			 * @param pose The pose to set for the color container.
			 * @param color The color to set for the color container.
			 * @param entities The entities to attach to the color container.
			 * @return A shared pointer to the newly created color container
			 * entity.
			 */
			std::shared_ptr<Container> makeColorContainer(
				Builder &builder, std::shared_ptr<Entity> parent,
				const utility::graphic::PoseF &pose,
				const utility::graphic::Color32Bit &color,
				const std::vector<std::shared_ptr<ecs::Entity>> &entities);
		};

		private:
		utility::graphic::PoseF
			_pose {};	 ///< Pose to be used for creating container entities
						 ///< (position, rotation, scale)
		utility::graphic::Color32Bit
			_color {};	  ///< Color to be used for creating container entities
						  ///< (RGBA)
		float _borderRadius { 16.0f };	  ///< Border radius to be used for
										  ///< creating container entities
		float _padding {
			16.0f
		};	  ///< Padding to be used around the
			  ///< children when creating container entities
		float _spacing { 8.0f };	///< Spacing to be used between children
									///< when creating container entities
		float _margin { 0.0f };	   ///< Margin to be used around the container
								   ///< when creating container entities
		Direction _direction { Direction::Row };	///< Direction in which
													///< children are arranged
		std::vector<std::shared_ptr<ecs::Entity>>
			_entities {};	 ///< Entities to be attached to created container
							 ///< entities
		static constexpr float _layerDepthStep {
			1.0f
		};	  ///< Distance pushed toward the camera per layer.

		public:
		/**
		 * @brief Default constructor for the Container component.
		 * @param registry Reference to the component registry for
		 * initializing components.
		 * @param pose The pose to initialize the Container component with
		 * (position, rotation, scale).
		 * @param color The color to initialize the Container component with
		 * (RGBA).
		 * @param borderRadius The border radius to initialize the Container
		 * component with.
		 * @param padding The padding to initialize the Container component
		 * with.
		 * @param spacing The spacing between children to initialize the
		 * Container component with.
		 * @param margin The margin around the container to initialize the
		 * Container component with.
		 * @param direction The direction in which children are arranged.
		 * @param entities The entities to attach to the container for this
		 * Container component.
		 */
		Container(ecs::ComponentRegistry &registry,
				  const utility::graphic::PoseF &pose,
				  const utility::graphic::Color32Bit &color, float borderRadius,
				  float padding, float spacing, float margin,
				  Direction direction,
				  const std::vector<std::shared_ptr<ecs::Entity>> &entities);

		/**
		 * @brief Default destructor for the Container component.
		 */
		~Container(void);

		/**
		 * @brief Set the pose of the container for this Container entity.
		 * @param pose The new pose to set for the container (position,
		 * rotation, scale).
		 * @return Reference to this Container for chaining.
		 */
		Container &setPose(const utility::graphic::PoseF &pose);

		/**
		 * @brief Set the color of the container for this Container entity.
		 * @param color The new color to set for the container (RGBA).
		 * @return Reference to this Container for chaining.
		 */
		Container &setColor(const utility::graphic::Color32Bit &color);

		/**
		 * @brief Set the border radius of the container for this Container
		 * entity.
		 * @param borderRadius The new border radius to set for the container.
		 * @return Reference to this Container for chaining.
		 */
		Container &setBorderRadius(float borderRadius);

		/**
		 * @brief Set the padding of the container for this Container entity.
		 * @param padding The new padding to set for the container.
		 * @return Reference to this Container for chaining.
		 */
		Container &setPadding(float padding);

		/**
		 * @brief Set the spacing between children for this Container entity.
		 * @param spacing The new spacing between children to set.
		 * @return Reference to this Container for chaining.
		 */
		Container &setSpacing(float spacing);

		/**
		 * @brief Set the margin around the container for this Container entity.
		 * @param margin The new margin around the container to set.
		 * @return Reference to this Container for chaining.
		 */
		Container &setMargin(float margin);

		/**
		 * @brief Set the direction in which children are arranged for this
		 * Container entity.
		 * @param direction The new direction to set.
		 * @return Reference to this Container for chaining.
		 */
		Container &setDirection(Direction direction);

		/**
		 * @brief Set the entities to be attached to the container for this
		 * Container entity.
		 * @param entities The new entities to attach to the container.
		 * @return Reference to this Container for chaining.
		 */
		Container &setEntities(
			const std::vector<std::shared_ptr<ecs::Entity>> &entities);

		/**
		 * @brief Initialize the container entity's derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the container entity's derived state.
		 */
		void update(void) override;

		/**
		 * @brief Apply the current layer depth offset to a position.
		 * @param position The base position.
		 * @param orientation The orientation.
		 * @param layer The container layer.
		 * @return The offset pose.
		 */
		static const utility::graphic::PoseF applyLayerToPosition(
			const utility::graphic::PositionF &position,
			const utility::graphic::OrientationF &orientation,
			const std::uint32_t &layer);

		private:
		/**
		 * @brief Arrange the container children in a row or a column, measure
		 * the container from them, and lift the children toward the camera to
		 * avoid z-fighting with the container surface.
		 */
		void applyGeometry(void);
	};
};	  // namespace guillaume::entities
