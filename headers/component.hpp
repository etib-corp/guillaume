/*
 Copyright (c) 2025 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <memory>
#include <vector>

/**
 * @class Component
 * @brief Represents a Component entity.
 *
 */
class Component : public std::enable_shared_from_this<Component>
{
private:
	typedef unsigned int ComponentID; ///< Type definition for ComponentID

	ComponentID _id; ///< The unique identifier for the component

	ComponentID generateID(void) const
	{
		static ComponentID currentID = 0;
		return ++currentID;
	}

	std::shared_ptr<Component> _parent;				   ///< Pointer to the parent component
	std::vector<std::shared_ptr<Component>> _children; ///< Vector of child components

protected:
	/**
	 * @brief Set the Parent object
	 *
	 * @param parent A shared pointer to the parent component
	 */
	void setParent(const std::shared_ptr<Component> &parent)
	{
		_parent = parent;
	}

public:
	/**
	 * @brief Constructs a Component object.
	 */
	Component(void) : _id(generateID()) {}

	/**
	 * @brief Destroy the Component object
	 *
	 */
	~Component(void) = default;

	/**
	 * @brief Get the ID object
	 *
	 * @return ComponentID
	 */
	ComponentID getID(void) const
	{
		return _id;
	}

	/**
	 * @brief Add a child component
	 *
	 * @param child A shared pointer to the child component
	 */
	void addChild(const std::shared_ptr<Component> &child)
	{
		child->setParent(shared_from_this());
		_children.push_back(child);
	}
};
