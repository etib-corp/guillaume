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

#include <cstddef>
#include <exception>
#include <string>

namespace guillaume::ecs
{

	/**
	 * @brief Maximum number of distinct component types in one program.
	 *
	 * It is the number of bits of Entity::Signature: each component type
	 * owns one bit, handed out by ComponentTypeId.
	 */
	constexpr std::size_t MaxComponentTypes = 128;

	/**
	 * @brief Thrown by ComponentTypeId::get when a program uses more than
	 * MaxComponentTypes component types.
	 */
	class ComponentTypeLimitExceededException: public std::exception
	{
		private:
		std::string _message;	 ///< Text returned by what()

		public:
		/**
		 * @brief Build the exception with its fixed message.
		 */
		ComponentTypeLimitExceededException(void);

		/**
		 * @brief Get the exception message.
		 * @return A null-terminated string owned by the exception.
		 */
		const char *what(void) const noexcept override;
	};

	/**
	 * @brief Gives each component type a small number: its bit position in
	 * an Entity::Signature.
	 *
	 * Numbers are handed out in the order the types are first asked for,
	 * starting at 0, and stay the same for the rest of the program. They are
	 * not stable from one run to the next, so never store them.
	 *
	 * @code
	 * Entity::Signature signature;
	 * signature.set(ComponentTypeId::get<components::Bound>());
	 * @endcode
	 *
	 * @see Entity::getSignatureFromTypes
	 * @see MaxComponentTypes
	 */
	class ComponentTypeId
	{
		public:
		/**
		 * @brief Get the number of a component type, assigning one on the
		 * first call for that type.
		 * @tparam ComponentType The component type.
		 * @return The number of the type, from 0 to MaxComponentTypes - 1.
		 * The same value is returned on every call for the same type.
		 * @throws ComponentTypeLimitExceededException On the first call for a
		 * type when MaxComponentTypes numbers are already in use.
		 */
		template<typename ComponentType> static std::size_t get(void)
		{
			static const std::size_t id = nextId();
			return id;
		}

		private:
		/**
		 * @brief Hand out the next unused number.
		 * @return The next number, starting at 0.
		 * @throws ComponentTypeLimitExceededException When MaxComponentTypes
		 * numbers were already handed out.
		 */
		static std::size_t nextId(void);
	};

}	 // namespace guillaume::ecs
