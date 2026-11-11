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
	 * @brief Key of the size cache of MeasureMesh: the path of the model
	 * file.
	 *
	 * Two entities showing the same file share one measurement. The
	 * ordering operator exists so the key can be used in the ordered map of
	 * utility::Cache.
	 * @see MeasureMesh
	 */
	struct MeasureMeshCacheKey {
		std::string meshPath;	 ///< Path of the model file that was measured

		/**
		 * @brief Compare two keys.
		 * @param other The key to compare with.
		 * @return True when both keys hold the same path.
		 */
		bool operator==(const MeasureMeshCacheKey &other) const
		{
			if (meshPath != other.meshPath) {
				return false;
			}
			return true;
		}

		/**
		 * @brief Negation of operator==.
		 * @param other The key to compare with.
		 * @return True when the paths differ.
		 */
		bool operator!=(const MeasureMeshCacheKey &other) const
		{
			return !(*this == other);
		}

		/**
		 * @brief Order two keys by path.
		 * @param other The key to compare with.
		 * @return True when this path sorts before the path of @p other.
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
	 * @brief Computes the size of every 3D model and writes it into its
	 * Bound, so the layout knows how much room the model takes.
	 *
	 * Runs in the Measure phase on entities with Mesh, Bound and Transform.
	 * - Reads: the model path of components::Mesh.
	 * - Writes: the width and height of components::Bound, taken from the
	 *   bounding size of the model
	 *   (utility::graphic::Model::computeBoundingSize).
	 *
	 * A model file is loaded and measured once per distinct path and the
	 * size is kept for the life of the system; later frames only copy the
	 * cached size.
	 *
	 * Things to know:
	 * - The Bound of a model entity is overwritten every frame.
	 * - The model must exist: the result of the load is used without a
	 *   check, unlike in systems::MeshRender.
	 *
	 * @see components::Mesh, systems::MeshRender
	 * @see MeasureMeshCacheKey
	 */
	class MeasureMesh:
		public utility::Cache<MeasureMeshCacheKey, utility::graphic::SizeF>,
		public ecs::SystemFiller<components::Mesh, components::Bound,
								 components::Transform>
	{
		private:
		std::shared_ptr<utility::RessourceProvider>
			_ressourceProvider;		 ///< Provider the models are loaded from
		utility::Engine &_engine;	 ///< Engine handle. Not used by the
									 ///< measurement

		public:
		/**
		 * @brief Create the system in the Measure phase, with an empty
		 * cache.
		 * @param ressourceProvider Provider used to load the models. Must
		 * not be null.
		 * @param engine The engine handle. It must outlive the system.
		 */
		MeasureMesh(
			std::shared_ptr<utility::RessourceProvider> ressourceProvider,
			utility::Engine &engine);

		/**
		 * @brief Destroy the system and its cache.
		 */
		~MeasureMesh(void);

		/**
		 * @brief Does nothing: the system needs no per-frame setup.
		 */
		void prepare(void) override;

		/**
		 * @brief Does nothing: the cache is kept from one frame to the next.
		 */
		void cleanup(void) override;

		/**
		 * @brief Write the size of one model into its components::Bound,
		 * loading and measuring the file when it is not in the cache yet.
		 * @param entityIdentifier The model entity. It is skipped, with a
		 * log message, when one of the three components is missing.
		 */
		virtual void
			update(const ecs::Entity::Identifier &entityIdentifier) override;
	};

}	 // namespace guillaume::systems
