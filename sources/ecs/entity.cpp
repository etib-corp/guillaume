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

#include "guillaume/ecs/entity.hpp"

#include <atomic>
#include <stdexcept>

#include "guillaume/components/hierarchy.hpp"
#include "guillaume/ecs/component_registry.hpp"

namespace guillaume::ecs
{

	namespace
	{
		/**
		 * @brief Write a layer to an entity and its descendants.
		 * @param registry The registry holding the Hierarchy components.
		 * @param identifier The entity at the top of the subtree.
		 * @param layer The layer of that entity.
		 */
		void propagateLayer(ComponentRegistry &registry,
							Entity::Identifier identifier, std::int32_t layer)
		{
			auto &hierarchy =
				registry.getComponent<components::Hierarchy>(identifier);

			hierarchy.setLayer(layer);
			for (const auto child: hierarchy.getChildIdentifiers()) {
				if (registry.hasComponent<components::Hierarchy>(child)) {
					propagateLayer(registry, child, layer + 1);
				}
			}
		}

		/**
		 * @brief Whether an entity is another one or one of its ancestors.
		 * @param registry The registry holding the Hierarchy components.
		 * @param ancestor The candidate ancestor.
		 * @param identifier The entity to walk up from.
		 * @return True when the walk up reaches the candidate.
		 */
		bool isSelfOrAncestor(const ComponentRegistry &registry,
							  Entity::Identifier ancestor,
							  Entity::Identifier identifier)
		{
			while (identifier != Entity::InvalidIdentifier) {
				if (identifier == ancestor) {
					return true;
				}
				if (!registry.hasComponent<components::Hierarchy>(identifier)) {
					return false;
				}
				identifier =
					registry.getComponent<components::Hierarchy>(identifier)
						.getParentIdentifier();
			}
			return false;
		}
	}	 // namespace

	Entity::Identifier Entity::getNextIdentifier(void)
	{
		static std::atomic<Identifier> currentId { InvalidIdentifier };
		return currentId.fetch_add(1, std::memory_order_relaxed) + 1;
	}

	Entity::Entity(void)
		: utility::logging::Loggable<Entity, utility::logging::DefaultLogger>()
		, _identifier(getNextIdentifier())
	{
	}

	Entity::Identifier Entity::getIdentifier(void) const noexcept
	{
		return _identifier;
	}

	Entity::Signature Entity::getSignature(void) const noexcept
	{
		return _signature;
	}

	void Entity::setSignature(const Signature &signature)
	{
		_signature = signature;
	}

	void Entity::attachHierarchy(ComponentRegistry &componentRegistry)
	{
		_hierarchyRegistry = &componentRegistry;
		if (!componentRegistry.hasComponent<components::Hierarchy>(
				_identifier)) {
			componentRegistry.addComponent<components::Hierarchy>(_identifier);
		}
		_signature |= getSignatureFromTypes<components::Hierarchy>();
	}

	std::int32_t Entity::getLayer(void) const noexcept
	{
		if (_hierarchyRegistry != nullptr
			&& _hierarchyRegistry->hasComponent<components::Hierarchy>(
				_identifier)) {
			const ComponentRegistry &registry = *_hierarchyRegistry;
			return registry.getComponent<components::Hierarchy>(_identifier)
				.getLayer();
		}
		return _layer;
	}

	Entity &Entity::setParent(std::shared_ptr<Entity> parent)
	{
		const Identifier parentIdentifier =
			parent != nullptr ? parent->getIdentifier() : InvalidIdentifier;
		const std::int32_t layer =
			parent != nullptr ? parent->getLayer() + 1 : 0;

		if (parent.get() == this) {
			throw std::invalid_argument("An entity cannot be its own parent");
		}
		if (_hierarchyRegistry == nullptr) {
			_parent = parent;
			_layer	= layer;
			return *this;
		}

		auto &registry = *_hierarchyRegistry;
		if (isSelfOrAncestor(registry, _identifier, parentIdentifier)) {
			throw std::invalid_argument(
				"An entity cannot be parented to one of its descendants");
		}

		auto &hierarchy =
			registry.getComponent<components::Hierarchy>(_identifier);
		const Identifier previous = hierarchy.getParentIdentifier();
		if (previous != parentIdentifier) {
			if (registry.hasComponent<components::Hierarchy>(previous)) {
				registry.getComponent<components::Hierarchy>(previous)
					.removeChild(_identifier);
			}
			if (registry.hasComponent<components::Hierarchy>(
					parentIdentifier)) {
				registry.getComponent<components::Hierarchy>(parentIdentifier)
					.addChild(_identifier);
			}
			hierarchy.setParentIdentifier(parentIdentifier);
		}
		_parent = parent;
		_layer	= layer;
		propagateLayer(registry, _identifier, layer);
		return *this;
	}

	std::shared_ptr<Entity> Entity::getParent(void) const noexcept
	{
		return _parent.lock();
	}

	void Entity::initialize(void)
	{
	}
}	 // namespace guillaume::ecs