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

/**
 * @class Composable
 * @brief An abstract base class for composable components.
 *
 * This class defines the interface for components that can be composed
 * together in a system.
 */
class Composable
{
private:
protected:
	/**
	 * @brief A type for component IDs
	 */
	using ComponentID = unsigned int;

public:
	/**
	 * @brief Destroy the Composable object
	 *
	 */
	virtual ~Composable(void) = default;

	/**
	 * @brief Get the ID of the component
	 *
	 * @return ComponentID The unique identifier of the component
	 */
	virtual ComponentID getID(void) const = 0;

	/**
	 * @brief Render the component
	 *
	 */
	virtual void render(void) const = 0;
};
