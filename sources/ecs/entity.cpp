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

namespace guillaume::ecs
{

	Entity::Identifier Entity::getNextIdentifier(void)
	{
		static Identifier currentId = InvalidIdentifier;
		return ++currentId;
	}

	Entity::Entity(void)
		: utility::logging::Loggable<Entity, utility::logging::DefaultLogger>()
		, _identifier(getNextIdentifier())
	{
	}

	Entity::Identifier Entity::getIdentifier(void) const
	{
		return _identifier;
	}

	Entity::Signature Entity::getSignature(void) const
	{
		return _signature;
	}

	void Entity::setSignature(const Signature &signature)
	{
		_signature = signature;
	}

	std::int32_t Entity::getLayer(void) const
	{
		return _layer;
	}

	Entity &Entity::setParent(std::shared_ptr<Entity> parent)
	{
		_parent = parent;
		if (parent != nullptr) {
			_layer = parent->getLayer() + 1;
		} else {
			_layer = 0;
		}
		return *this;
	}

	std::shared_ptr<Entity> Entity::getParent(void) const
	{
		return _parent;
	}
}	 // namespace guillaume::ecs