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

#include <utility/ressource_provider.hpp>

#include <utility/cache.hpp>

#include <utility/graphic/model.hpp>

#include <utility/engine.hpp>

#include "guillaume/ecs/system_filler.hpp"

#include "guillaume/components/bound.hpp"
#include "guillaume/components/mesh.hpp"
#include "guillaume/components/transform.hpp"

namespace guillaume::systems
{
	/**
	 * @brief Structure representing a cache key for measuring models.
	 */
	struct MeasureMeshCacheKey {
		std::string meshPath;	 ///< The path of the model to be measured

		/**
		 * @brief Equality operator for MeasureMeshCacheKey.
		 * @param other The other MeasureMeshCacheKey to compare with.
		 * @return True if the keys are equal, false otherwise.
		 */
		bool operator==(const MeasureMeshCacheKey &other) const
		{
			if (meshPath != other.meshPath) {
				return false;
			}
			return true;
		}

		/**
		 * @brief Inequality operator for MeasureMeshCacheKey.
		 * @param other The other MeasureMeshCacheKey to compare with.
		 * @return True if the keys are not equal, false otherwise.
		 */
		bool operator!=(const MeasureMeshCacheKey &other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Less-than operator for MeasureMeshCacheKey.
		 * @param other The other MeasureMeshCacheKey to compare with.
		 * @return True if this key is less than the other, false otherwise.
		 */
		bool operator<(const MeasureMeshCacheKey &other) const
		{
			if (meshPath != other.meshPath) {
				return meshPath < other.meshPath;
			}
			return false;
		}
	};

	/**
	 * @brief System measuring models and synchronizing them to bound sizes.
	 * @see components::Mesh
	 * @see components::Bound
	 * @see components::Transform
	 */
	class MeasureMesh:
		public utility::Cache<MeasureMeshCacheKey, utility::graphic::SizeF>,
		public ecs::SystemFiller<components::Mesh, components::Bound,
								 components::Transform>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;	   ///< Shared resource provider for loading
								   ///< fonts and models
		utility::Engine
			&_engine;	 ///< utility::Engine instance for text measurement

		public:
		/**
		 * @brief Construct a model measuring system.
		 * @param ressourceProvider Shared resource provider for loading fonts
		 * and models.
		 * @param engine The engine used to measure text.
		 */
		MeasureMesh(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			utility::Engine &engine);

		/**
		 * @brief Default destructor.
		 */
		~MeasureMesh(void);

		/**
		 * @brief Prepare the MeasureMesh system before measurement.
		 *
		 * This function is called before call update on all entities and can be
		 * used to set up any necessary state or resources.
		 */
		void prepare(void) override;

		/**
		 * @brief Clean up the MeasureMesh system after measurement.
		 *
		 * This function is called after call update on all entities and can be
		 * used to release any resources or reset state.
		 */
		void cleanup(void) override;

		/**
		 * @brief Update the MeasureMesh system for one entity.
		 * @param entityIdentifier The target entity identifier.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
