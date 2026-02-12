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

namespace guillaume::ecs {

/**
 * @brief Maximum number of distinct component types supported by signatures.
 */
constexpr std::size_t MaxComponentTypes = 64;

/**
 * @brief Exception thrown when the component type limit is exceeded.
 */
class ComponentTypeLimitExceededException : public std::exception {
  private:
    std::string _message;

  public:
    /**
     * @brief Construct a new Component Type Limit Exceeded Exception.
     */
    ComponentTypeLimitExceededException(void)
        : _message("Exceeded maximum number of component types") {}

    /**
     * @brief Get the exception message.
     * @return The exception message.
     */
    const char *what(void) const noexcept override { return _message.c_str(); }
};

/**
 * @brief Monotonic component type id generator.
 */
class ComponentTypeId {
  public:
    /**
     * @brief Get a stable id for a component type.
     * @tparam ComponentType The component type.
     * @return A stable id within [0, MaxComponentTypes).
     */
    template <typename ComponentType> static std::size_t get(void) {
        static const std::size_t id = nextId();
        return id;
    }

  private:
    /**
     * @brief Generate the next available id.
     * @return The next id.
     */
    static std::size_t nextId(void) {
        static std::size_t currentId = 0;
        if (currentId >= MaxComponentTypes) {
            throw ComponentTypeLimitExceededException();
        }
        return currentId++;
    }
};

} // namespace guillaume::ecs
