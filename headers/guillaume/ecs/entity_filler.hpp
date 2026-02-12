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

#include "guillaume/ecs/component.hpp"
#include "guillaume/ecs/component_registry.hpp"
#include "guillaume/ecs/entity.hpp"

namespace guillaume::ecs {

/**
 * @brief Templated Entity class that automatically sets its signature based
 * on the specified component types.
 * @tparam ComponentTypes The component types that define the entity's
 * signature.
 *
 * @code
 * // UI-style entity composed from common components.
 * class ButtonEntity
 *     : public ecs::EntityFiller<components::Transform,
 *                                components::Bound,
 *                                components::Click,
 *                                components::Text> {
 *   public:
 *     ButtonEntity(ecs::ComponentRegistry &registry)
 *         : ecs::EntityFiller<components::Transform,
 *                             components::Bound,
 *                             components::Click,
 *                             components::Text>(registry) {}
 * };
 *
 * ButtonEntity button(registry);
 * @endcode
 *
 * @see Entity
 * @see ComponentRegistry
 */
template <InheritFromComponent... ComponentTypes>
class EntityFiller : public Entity {
  private:
    ComponentRegistry &_componentRegistry;

  protected:
    /**
     * @brief Get the Component Registry.
     * @return Reference to the component registry.
     */
    ComponentRegistry &getComponentRegistry(void) { return _componentRegistry; }

  public:
    /**
     * @brief Construct a new Entity Filler object.
     * @param componentRegistry The component registry to register components
     * to.
     */
    EntityFiller(ComponentRegistry &componentRegistry)
        : Entity(), _componentRegistry(componentRegistry) {
        setSignature<ComponentTypes...>();
        _componentRegistry
            .template registerComponentsForEntity<ComponentTypes...>(
                getIdentifier());
    }

    /**
     * @brief Default destructor for the Entity Filler class.
     */
    virtual ~EntityFiller(void) = default;
};

} // namespace guillaume::ecs