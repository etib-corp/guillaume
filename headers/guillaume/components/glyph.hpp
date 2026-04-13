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

#include <string>

#include <utility/graphic/color.hpp>

#include "guillaume/ecs/component.hpp"

namespace guillaume::components
{

	/**
	 * @brief Component representing a glyph element.
	 */
	class Glyph: public ecs::Component
	{
		private:
		uint32_t _code { 0 };  ///< Glyph code (e.g., Unicode code point)
		std::string _name {};  ///< Glyph name (for font lookup)

		public:
		/**
		 * @brief Default constructor for the Glyph component.
		 */
		Glyph(void) = default;

		/**
		 * @brief Default destructor for the Glyph component.
		 */
		~Glyph(void) = default;

		/**
		 * @brief Get the glyph code.
		 * @return The glyph code.
		 */
		uint32_t getCode(void) const
		{
			return _code;
		}

		/**
		 * @brief Set the glyph code.
		 * @param code The new glyph code.
		 * @return The Glyph component for chaining.
		 */
		Glyph &setCode(uint32_t code)
		{
			if (_code == code) {
				return *this;
			}
			_code = code;
			setHasChanged(true);
			return *this;
		}

		/**
		 * @brief Get the glyph name.
		 * @return The glyph name.
		 */
		const std::string &getName(void) const
		{
			return _name;
		}

		/**
		 * @brief Set the glyph name.
		 * @param name The new glyph name.
		 * @return The Glyph component for chaining.
		 */
		Glyph &setName(const std::string &name)
		{
			if (_name == name) {
				return *this;
			}
			_name = name;
			setHasChanged(true);
			return *this;
		}
	};

}	 // namespace guillaume::components