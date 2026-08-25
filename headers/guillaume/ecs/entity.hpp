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

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <memory>

#include <utility/logging/loggable.hpp>
#include <utility/logging/default_logger.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/component_type_id.hpp"

namespace guillaume::ecs
{

	/**
	 * @brief Non-template base class for entities in the ECS architecture.
	 *
	 * This base class provides common functionality that doesn't depend on
	 * component types.
	 * @see ComponentRegistry
	 * @see System
	 */
	class Entity:
		protected utility::logging::Loggable<Entity,
											 utility::logging::DefaultLogger>
	{
		public:
		/**
		 * @brief Type alias for entity identifiers.
		 */
		using Identifier = std::size_t;
		/**
		 * @brief Type alias for entity signatures (64-bit component bitmask).
		 */
		using Signature = std::bitset<64>;

		constexpr static Identifier InvalidIdentifier =
			0;	  ///< Invalid identifier

		private:
		/**
		 * @brief Generate the next unique identifier for an entity.
		 * @return The next unique identifier.
		 */
		static Identifier getNextIdentifier(void);

		public:
		/**
		 * @brief Generate a signature from the given component types.
		 * @tparam NeededComponents The component types.
		 * @return The generated signature.
		 */
		template<InheritFromComponent... ComponentTypes>
		static Signature getSignatureFromTypes(void);

		private:
		const Identifier _identifier;		///< Unique identifier
		Signature _signature;				///< Entity signature
		std::int32_t _layer { 0 };			///< Rendering/depth layer
		std::shared_ptr<Entity> _parent;	///< Parent entity (if any)

		protected:
		/**
		 * @brief Set the entity's signature based on the specified component
		 * types.
		 * @tparam ComponentTypes The component types that define the entity's
		 * signature.
		 */
		template<InheritFromComponent... ComponentTypes>
		void setSignature(void);

		public:
		/**
		 * @brief Default constructor for the Entity class.
		 */
		Entity(void);

		/**
		 * @brief Default destructor for the Entity class.
		 */
		virtual ~Entity(void) = default;

		/**
		 * @brief Get the unique identifier of the entity.
		 * @return The entity's unique identifier.
		 */
		Entity::Identifier getIdentifier(void) const;

		/**
		 * @brief Get the entity's signature.
		 * @return The entity's signature.
		 */
		Entity::Signature getSignature(void) const;

		/**
		 * @brief Set the entity's signature.
		 * @param signature The new signature.
		 */
		void setSignature(const Signature &signature);

		/**
		 * @brief Get the entity's rendering layer.
		 * @return The layer value.
		 */
		std::int32_t getLayer(void) const;

		/**
		 * @brief Set the parent entity.
		 * @param parent The new parent entity.
		 * @return Reference to this Entity for chaining.
		 */
		Entity &setParent(std::shared_ptr<Entity> parent);

		/**
		 * @brief Get the parent entity.
		 * @return The parent entity, or nullptr if there is no parent.
		 */
		std::shared_ptr<Entity> getParent(void) const;

		/**
		 * @brief Initialize the entity's derived state.
		 *
		 * This method is called after the entity is constructed and its
		 * components are set up. Derived entities can override this method to
		 * perform any necessary initialization based on their components.
		 */
		virtual void initialize(void) = 0;

		/**
		 * @brief Recompute the entity's derived state.
		 *
		 * The default implementation is a no-op. Derived entities can override
		 * this method to synchronize cached state or dependent components when
		 * their component data changes.
		 */
		virtual void update(void) = 0;
	};

	/**
	 * @brief Concept to ensure a type inherits from Entity.
	 * @tparam Type The type to check.
	 */
	template<typename Type>
	concept InheritFromEntity = std::is_base_of_v<Entity, Type>;

}	 // namespace guillaume::ecs

// Include the implementation of the Entity template methods
#include "guillaume/ecs/entity.tpp"