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
#include <functional>
#include <typeindex>
#include <vector>

#include "ecs/component.hpp"
#include "ecs/entity.hpp"

#include "event_bus.hpp"

namespace guillaume {
class EventBus;
class Event;
enum class EventType;
} // namespace guillaume

namespace guillaume::ecs {

/**
 * @brief Base class for all systems in the ECS architecture.
 *
 * This base class provides common functionality that doesn't depend on
 * component types.
 */
class System {
  private:
    Entity::Signature _signature;              ///< System signature
    std::vector<Entity::Identifier> _entities; ///< Managed entities
    std::queue<guillaume::Event> _eventQueue;  ///< Event queue

  protected:
    /**
     * @brief Set the system's signature based on the specified component types.
     * @tparam ComponentTypes The component types that define the system's
     * signature.
     */
    template <InheritFromComponent... ComponentTypes> void setSignature(void) {
        _signature =
            (std::type_index(typeid(ComponentTypes)).hash_code() | ...);
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
    void addEntity(Entity::Identifier entityIdentifier) {
        _entities.push_back(entityIdentifier);
    }

    /**
     * @brief Routine to update all managed entities.
     */
    void routine(void) {
        for (const auto &entityIdentifier : _entities) {
            update(entityIdentifier);
        }
    }

    /**
     * @brief Update the system, processing relevant entities.
     * @param identityIdentifier The identifier of the entity to update.
     */
    virtual void update(const Entity::Identifier &identityIdentifier) = 0;

    /**
     * @brief Subscribe to a specific event type on the event bus.
     * @param eventBus The event bus to subscribe to.
     * @param eventType The event type to listen for.
     * @param callback The callback to invoke when the event is published.
     */
    void subscribeToEvent(EventBus &eventBus, EventType eventType,
                          const std::function<void(Event &)> &callback) {
        eventBus.subscribe(eventType, callback);
    }
};

/**
 * @brief Concept to ensure a type inherits from System.
 * @tparam Type The type to check.
 */
template <typename Type>
concept InheritFromSystem = std::is_base_of_v<System, Type>;

} // namespace guillaume::ecs