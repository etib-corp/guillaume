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

#include <algorithm>
#include <bitset>
#include <functional>
#include <vector>

#include <utility/logging/loggable.hpp>
#include <utility/logging/standard_logger.hpp>

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/component_type_id.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs {

/**
 * @brief Base class for all systems in the ECS architecture.
 *
 * This base class provides common functionality that doesn't depend on
 * component types.
 */
class System
    : protected utility::logging::Loggable<System,
                                           utility::logging::StandardLogger> {
  private:
    Entity::Signature _signature;              ///< System signature
    std::vector<Entity::Identifier> _entities; ///< Managed entities

  protected:
    /**
     * @brief Set the system's signature based on the specified component types.
     * @tparam ComponentTypes The component types that define the system's
     * signature.
     */
    template <InheritFromComponent... ComponentTypes> void setSignature(void) {
        _signature.reset();
        (_signature.set(ComponentTypeId::get<ComponentTypes>()), ...);
    }

  public:
    /**
     * @brief Default constructor for the System class.
     */
    System(void) = default;

    /**
     * @brief Default destructor for the System class.
     */
    virtual ~System(void) = default;

    /**
     * @brief Get the system's signature.
     * @return The system's signature.
     */
    Entity::Signature getSignature(void) const { return _signature; }

    /**
     * @brief Add an entity to be managed by the system.
     * @param entityIdentifier The identifier of the entity to add.
     */
    void addEntity(Entity::Identifier entityIdentifier);

    /**
     * @brief Remove an entity from the system.
     * @param entityIdentifier The identifier of the entity to remove.
     */
    void removeEntity(Entity::Identifier entityIdentifier);

    /**
     * @brief Check if an entity is already managed.
     * @param entityIdentifier The identifier of the entity.
     * @return True if the entity is present.
     */
    bool hasEntity(Entity::Identifier entityIdentifier) const;

    /**
     * @brief Routine to update all managed entities.
     * @param componentRegistry The component registry instance.
     */
    void routine(ecs::ComponentRegistry &componentRegistry);

    /**
     * @brief Update the system, processing relevant entities.
     * @param componentRegistry The component registry instance.
     * @param identityIdentifier The identifier of the entity to update.
     */
    virtual void update(ecs::ComponentRegistry &componentRegistry,
                        const ecs::Entity::Identifier &identityIdentifier) = 0;
};

/**
 * @brief Concept to ensure a type inherits from System.
 * @tparam Type The type to check.
 */
template <typename Type>
concept InheritFromSystem = std::is_base_of_v<System, Type>;

} // namespace guillaume::ecs