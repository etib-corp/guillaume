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
#include <typeindex>

#include "guillaume/ecs/component.hpp"

namespace guillaume::ecs {

/**
 * @brief Non-template base class for entities in the ECS architecture.
 *
 * This base class provides common functionality that doesn't depend on
 * component types.
 */
class Entity {
  public:
    using Identifier = std::size_t;    ///< Type alias for entity identifiers
    using Signature = std::bitset<64>; ///< Type alias for entity signatures

    constexpr static Identifier InvalidIdentifier = 0; ///< Invalid identifier

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
    template <InheritFromComponent... ComponentTypes>
    static Signature getSignatureFromTypes(void) {
        return (std::type_index(typeid(ComponentTypes)).hash_code() | ...);
    }

  private:
    const Identifier _identifier; ///< Unique identifier
    Signature _signature;         ///< Entity signature

  protected:
    /**
     * @brief Set the entity's signature based on the specified component types.
     * @tparam ComponentTypes The component types that define the entity's
     * signature.
     */
    template <InheritFromComponent... ComponentTypes> void setSignature(void) {
        _signature = getSignatureFromTypes<ComponentTypes...>();
    }

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
    Entity::Identifier getIdentifier(void) const { return _identifier; }

    /**
     * @brief Get the entity's signature.
     * @return The entity's signature.
     */
    Entity::Signature getSignature(void) const { return _signature; }
};

/**
 * @brief Concept to ensure a type inherits from System.
 * @tparam Type The type to check.
 */
template <typename Type>
concept InheritFromEntity = std::is_base_of_v<Entity, Type>;

} // namespace guillaume::ecs