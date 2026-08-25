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

#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity_director.hpp"
#include "guillaume/ecs/entity_builder.hpp"
#include "guillaume/ecs/entity_filler.hpp"

#include "guillaume/components/model.hpp"
#include "guillaume/components/bound.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::entities
{

	/**
	 * @brief Model component
	 */
	class Model:
		public ecs::EntityFiller<components::Transform, components::Bound,
								 components::Model>
	{
		public:
		/**
		 * @brief Builder used to configure and create `Model` entities.
		 */
		class Builder: public ecs::EntityBuilder
		{
			private:
			std::shared_ptr<Model>
				_model;	   ///< Unique pointer to the Model entity being built
			std::string _modelPath;		 ///< Name of the model to be used for
										 ///< this Model entity
			std::string _texturePath;	 ///< Name of the texture to be used for
										 ///< this Model entity

			public:
			/**
			 * @brief Construct a new Model Builder object.
			 * @param componentRegistry The component registry to register
			 * components to.
			 * @param entityRegistry The entity registry to register entities
			 * to.
			 */
			Builder(ecs::ComponentRegistry &componentRegistry,
					ecs::EntityRegistry &entityRegistry);

			/**
			 * @brief Default destructor for the Model Builder class.
			 */
			~Builder(void);

			/**
			 * @brief Build and register the model entity.
			 * @param parent The parent entity to which the new model entity
			 * will be attached.
			 * @return A shared pointer to the newly created model entity.
			 */
			std::shared_ptr<Model>
				registerEntity(std::shared_ptr<Entity> parent);

			/**
			 * @brief Reset the builder to its initial state for creating a new
			 * Model entity.
			 */
			void reset(void) override;

			/**
			 * @brief Set the name of the model to be used for the Model entity.
			 * @param modelPath The model identifier to set.
			 * @return Reference to the builder for chaining.
			 * @see components::Model::getName
			 */
			Builder &withModelPath(const std::string &modelPath);

			/**
			 * @brief Set the name of the texture to be used for the Model
			 * entity.
			 * @param texturePath The texture identifier to set.
			 * @return Reference to the builder for chaining.
			 */
			Builder &withTexturePath(const std::string &texturePath);
		};

		/**
		 * @brief Director that orchestrates `Model::Builder` to create
		 * preconfigured model entities.
		 */
		class Director: public ecs::EntityDirector
		{
			public:
			/**
			 * @brief Construct a new Model Director object.
			 */
			Director(void);

			/**
			 * @brief Default destructor for the Model Director class.
			 */
			~Director(void);

			/**
			 * @brief Create a default model entity using the builder.
			 * @param builder The builder instance used to configure and create
			 * the default model.
			 * @param parent The parent entity to which the new model entity
			 * will be attached.
			 * @param modelPath The model name to assign to the created entity.
			 * @param texturePath The texture name to assign to the created
			 * entity (optional).
			 * @return The entity identifier of the newly created model entity.
			 * @see components::Glyph::getName
			 */
			std::shared_ptr<Model>
				makeModel(Builder &builder, std::shared_ptr<Entity> parent,
						  const std::string &modelPath,
						  const std::string &texturePath = "");
		};

		private:
		std::string _modelPath;	   ///< Name of the model to be used for this
								   ///< Model entity

		std::string _texturePath;	 ///< Name of the texture to be used for
									 ///< this Model entity

		public:
		/**
		 * @brief Default constructor for the Model component.
		 * @param registry Reference to the component registry for initializing
		 * components.
		 * @param modelPath The name of the model to be used for this Model
		 * entity.
		 * @see components::Model::getName
		 */
		Model(ecs::ComponentRegistry &registry, const std::string &modelPath);

		/**
		 * @brief Default destructor for the Model component.
		 */
		~Model(void);

		/**
		 * @brief Set the name of the model for this Model entity.
		 * @param modelPath The new name of the model to set.
		 * @return Reference to this Model for chaining.
		 */
		Model &setModelPath(const std::string &modelPath);

		/**
		 * @brief Set the name of the texture for this Model entity.
		 * @param texturePath The new name of the texture to set.
		 * @return Reference to this Model for chaining.
		 */
		Model &setTexturePath(const std::string &texturePath);

		/**
		 * @brief Initialize the model entity's derived state.
		 */
		void initialize(void) override;

		/**
		 * @brief Recompute the model entity's derived state.
		 */
		void update(void) override;
	};

}	 // namespace guillaume::entities
